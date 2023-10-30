#include "vn_networksystem.h"

#include "vn_protocolevent.h"
#include "vn_networkstatusevent.h"
#include "ecs_schema_generated.h"
#include <flatbuffers/flatbuffers.h>

#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_errordata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_remoteproceduredata.h>
#include <ve_commandevent.h>

#include <QUuid>

Q_LOGGING_CATEGORY(VEIN_NET, VEIN_DEBUGNAME_NET)
Q_LOGGING_CATEGORY(VEIN_NET_VERBOSE, VEIN_DEBUGNAME_NET_VERBOSE)

using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinNet
{
class NetworkSystemPrivate
{
    // stands for QHash<"entity descriptor", QList<"network id"> *>
    template <typename T>
    using SubscriptionStorage = QHash<T, QList<QUuid>>;

    explicit NetworkSystemPrivate(NetworkSystem *t_qPtr) :
        q_ptr(t_qPtr)
    {
        VF_ASSERT(static_cast<int>(CommandEvent::EventSubtype::NOTIFICATION) == static_cast<int>(VeinFrameworkIDL::EventSubtype_VC_NOTIFICATION), "Enum compatibility is a requirement");
        VF_ASSERT(static_cast<int>(CommandEvent::EventSubtype::TRANSACTION) == static_cast<int>(VeinFrameworkIDL::EventSubtype_VC_TRANSACTION), "Enum compatibility is a requirement");
    }

    void processIncoming(QEvent *t_event)
    {
        ProtocolEvent *pEvent = static_cast<ProtocolEvent *>(t_event);
        Q_ASSERT(pEvent != nullptr);
        //do not process messages from this instance
        if(pEvent->isOfLocalOrigin() == false) {
            QByteArray rawBuffer = pEvent->buffer();
            flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t *>(rawBuffer.constData()), rawBuffer.size());
            if(VeinFrameworkIDL::VerifyECSEnvelopeBuffer(verifier)) {
                const VeinFrameworkIDL::ECSEnvelope *ecsFlatBuffer = VeinFrameworkIDL::GetECSEnvelope(rawBuffer.data());
                auto ecsEventVector = ecsFlatBuffer->ecsEvents();
                for(flatbuffers::uoffset_t i=0; i < ecsEventVector->size(); ++i) {
                    const VeinFrameworkIDL::ECSEvent *entityEvent = ecsEventVector->Get(i);
                    VeinEvent::EventData *evData = nullptr;
                    const char *eventDataArray = reinterpret_cast<const char *>(entityEvent->eventData()->data());
                    const int eventDataArraySize = entityEvent->eventData()->size();

                    switch(entityEvent->dataType())
                    {
                    case VeinComponent::EntityData::dataType():
                    {
                        VeinComponent::EntityData * tmpData = new VeinComponent::EntityData();
                        tmpData->deserialize(QByteArray(eventDataArray, eventDataArraySize));
                        evData = tmpData;
                        break;
                    }
                    case VeinComponent::ErrorData::dataType():
                    {
                        VeinComponent::ErrorData *tmpData = new VeinComponent::ErrorData();
                        tmpData->deserialize(QByteArray(eventDataArray, eventDataArraySize));
                        evData = tmpData;
                        break;
                    }
                    case VeinComponent::ComponentData::dataType():
                    {
                        VeinComponent::ComponentData * tmpData = new VeinComponent::ComponentData();
                        tmpData->deserialize(QByteArray(eventDataArray, eventDataArraySize));
                        evData = tmpData;
                        break;
                    }
                    case VeinComponent::IntrospectionData::dataType():
                    {
                        VeinComponent::IntrospectionData *tmpData = new VeinComponent::IntrospectionData();
                        tmpData->deserialize(QByteArray(eventDataArray, eventDataArraySize));
                        evData = tmpData;
                        break;
                    }
                    case VeinComponent::RemoteProcedureData::dataType():
                    {
                        VeinComponent::RemoteProcedureData *tmpData = new VeinComponent::RemoteProcedureData();
                        tmpData->deserialize(QByteArray(eventDataArray, eventDataArraySize));
                        evData = tmpData;
                        break;
                    }
                    }
                    VF_ASSERT(evData != 0, "Unhandled event datatype");
                    if(evData->isValid()) {
                        evData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_FOREIGN);
                        CommandEvent *tmpEvent = new CommandEvent(static_cast<CommandEvent::EventSubtype>(entityEvent->command()), evData); //enums are compatible
                        tmpEvent->setPeerId(pEvent->peerId());
                        vCDebug(VEIN_NET_VERBOSE) << "Processing ProtocolEvent:" << pEvent << "new event:" << tmpEvent;
                        emit q_ptr->sigSendEvent(tmpEvent);
                    }
                    else
                        qCWarning(VEIN_NET) << "Received invalid event from FlatBuffer:" << QByteArray(eventDataArray, eventDataArraySize).toBase64();
                }
            }
        }
    }
    void processOutgoing(QEvent *t_event)
    {
        VeinEvent::CommandEvent *cEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
        Q_ASSERT(cEvent != nullptr);
        VeinEvent::EventData *evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);
        switch (m_operationMode)
        {
        case VeinNet::NetworkSystem::VNOM_DEBUG:
            vCDebug(VEIN_NET_VERBOSE) << "Debug mode is enabled, dropped event:" << t_event;
            t_event->accept();
            break;
        case VeinNet::NetworkSystem::VNOM_PASS_THROUGH:
            if(evData->eventOrigin() == VeinEvent::EventData::EventOrigin::EO_LOCAL
                && evData->eventTarget() == VeinEvent::EventData::EventTarget::ET_ALL) {
                QByteArray flatBuffer = prepareEnvelope(cEvent);
                QList<QUuid> protoReceivers;
                if(cEvent->peerId().isNull() == false)
                    protoReceivers = QList<QUuid>() << cEvent->peerId();
                sendNetworkEvent(protoReceivers, flatBuffer);
            }
            break;
        case VeinNet::NetworkSystem::VNOM_SUBSCRIPTION:
            // check if the event is a notification event with entity command subscribe/unsubscribe
            //   drop the event and add/remove the sender to/from the subscriber list
            // or else
            //   send the event to all active subscribers
            if(evData->eventOrigin() == VeinEvent::EventData::EventOrigin::EO_LOCAL
                && evData->eventTarget() == VeinEvent::EventData::EventTarget::ET_ALL) {
                bool handled = false;
                if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION && evData->type() == VeinComponent::EntityData::dataType())
                    handled = tryHandleSubscription(static_cast<VeinComponent::EntityData *>(evData), cEvent->peerId());
                if(handled)
                    cEvent->setAccepted(true);
                else if(m_subscriptions.contains(evData->entityId())) {
                    QList<QUuid> protoReceivers=m_subscriptions.value(evData->entityId());
                    if(protoReceivers.isEmpty() == false) {
                        QByteArray flatBuffer = prepareEnvelope(cEvent);
                        vCDebug(VEIN_NET_VERBOSE) << "Processing command event:" << cEvent << "type:" << static_cast<qint8>(cEvent->eventSubtype());// << "new event:" << protoEvent;
                        sendNetworkEvent(protoReceivers, flatBuffer);
                    }
                }
            }
            break;
        }

    }

    bool tryHandleSubscription(VeinComponent::EntityData *t_eData, QUuid t_peerId)
    {
        Q_ASSERT(t_eData != nullptr);
        bool handled = false;
        switch(t_eData->eventCommand())
        {
        case EntityData::Command::ECMD_SUBSCRIBE:
        {
            QList<QUuid> tmpCurrentSubscriptions = m_subscriptions.value(t_eData->entityId());
            if(!tmpCurrentSubscriptions.contains(t_peerId))
                tmpCurrentSubscriptions.append(t_peerId);
            m_subscriptions.insert(t_eData->entityId(), tmpCurrentSubscriptions);
            vCDebug(VEIN_NET_VERBOSE) << "Added subscription for entity:" << t_eData->entityId() << "network peer:" << t_peerId;
            handled = true;
            break;
        }
        case EntityData::Command::ECMD_UNSUBSCRIBE:
        {
            QList<QUuid> tmpCurrentSubscriptions = m_subscriptions.value(t_eData->entityId());
            tmpCurrentSubscriptions.removeAll(t_peerId);
            m_subscriptions.insert(t_eData->entityId(), tmpCurrentSubscriptions);
            vCDebug(VEIN_NET_VERBOSE) << "Removed subscription for entity:" << t_eData->entityId() << "network peer:" << t_peerId;
            handled = true;
            break;
        }
        default:
            break;
        }
        return handled;
    }

    void handleNetworkStatusEvent(NetworkStatusEvent *t_sEvent)
    {
        Q_ASSERT(t_sEvent != nullptr);
        vCDebug(VEIN_NET_VERBOSE) << "processing NetworkStatusEvent:" << t_sEvent;
        if(t_sEvent->getStatus() == NetworkStatusEvent::NetworkStatus::NSE_DISCONNECTED) {
            const QUuid tmpPeerId = t_sEvent->getPeerId();
            const auto tmpSubscriptionKeysCopy = m_subscriptions.keys();
            for(const int tmpKey : tmpSubscriptionKeysCopy) {
                QList<QUuid> tmpSubscribers = m_subscriptions.value(tmpKey);
                if(tmpSubscribers.contains(tmpPeerId)) {
                    tmpSubscribers.removeAll(tmpPeerId);
                    m_subscriptions.insert(tmpKey, tmpSubscribers);
                    vCDebug(VEIN_NET_VERBOSE) << "Removed subscription for entity:" << tmpKey << "for disconnected network peer:" << tmpPeerId;
                }
            }
        }
        else if(t_sEvent->getStatus() == NetworkStatusEvent::NetworkStatus::NSE_SOCKET_ERROR) {
            switch(t_sEvent->getError())
            {
            case QAbstractSocket::RemoteHostClosedError:
                break; /// @todo reconnect here?
            default:
                break;
            }
        }
    }

    QByteArray prepareEnvelope(VeinEvent::CommandEvent *t_cEvent)
    {
        Q_ASSERT(t_cEvent != nullptr);
        const VeinEvent::EventData *evData = t_cEvent->eventData();
        Q_ASSERT(evData != nullptr);
        const QByteArray serializedEventData = evData->serialize();
        const auto dataString = m_flatBufferBuilder.CreateString(serializedEventData.constData(), serializedEventData.size());

        VeinFrameworkIDL::ECSEventBuilder ecsEventBuilder = VeinFrameworkIDL::ECSEventBuilder(m_flatBufferBuilder);
        ecsEventBuilder.add_command(static_cast<VeinFrameworkIDL::EventSubtype>(t_cEvent->eventSubtype())); //enums are compatible

        ecsEventBuilder.add_dataType(evData->type());
        ecsEventBuilder.add_eventData(dataString);

        const std::vector<flatbuffers::Offset<VeinFrameworkIDL::ECSEvent>> tmpEventVector = {ecsEventBuilder.Finish()};
        const auto eventVector = m_flatBufferBuilder.CreateVector<flatbuffers::Offset<VeinFrameworkIDL::ECSEvent>>(tmpEventVector);

        VeinFrameworkIDL::ECSEnvelopeBuilder ecsEnvelopeBuilder = VeinFrameworkIDL::ECSEnvelopeBuilder(m_flatBufferBuilder);
        ecsEnvelopeBuilder.add_ecsEvents(eventVector);
        const auto rootElement = ecsEnvelopeBuilder.Finish();
        m_flatBufferBuilder.Finish(rootElement);

        QByteArray retVal = QByteArray(reinterpret_cast<const char*>(m_flatBufferBuilder.GetBufferPointer()), static_cast<int>(m_flatBufferBuilder.GetSize()));
        m_flatBufferBuilder.Clear();
        return retVal;
    }

    void sendNetworkEvent(QList<QUuid> t_receivers, QByteArray t_data)
    {
        Q_ASSERT(t_data.isNull() == false);
        ProtocolEvent *protoEvent = new ProtocolEvent(ProtocolEvent::EventOrigin::EO_LOCAL); //create a new event of local origin
        protoEvent->setBuffer(t_data);
        protoEvent->setReceivers(t_receivers);
        emit q_ptr->sigSendEvent(protoEvent);
    }

    NetworkSystem::OperationMode m_operationMode = NetworkSystem::VNOM_SUBSCRIPTION;

    /**
     * @brief stores current subscribers
     */
    SubscriptionStorage<int> m_subscriptions;

    flatbuffers::FlatBufferBuilder m_flatBufferBuilder;

    NetworkSystem *q_ptr;

    friend class NetworkSystem;
};



NetworkSystem::NetworkSystem(QObject * t_parent) :
    EventSystem(t_parent),
    d_ptr(new NetworkSystemPrivate(this))
{
    vCDebug(VEIN_NET) << "Initialized network system";
}

NetworkSystem::~NetworkSystem()
{
    vCDebug(VEIN_NET) << "Deinitialized network system";
    delete d_ptr;
}

NetworkSystem::OperationMode NetworkSystem::operationMode() const
{
    return d_ptr->m_operationMode;
}

void NetworkSystem::setOperationMode(const NetworkSystem::OperationMode &t_operationMode)
{
    d_ptr->m_operationMode = t_operationMode;
}

void NetworkSystem::processEvent(QEvent *event)
{
    Q_ASSERT(event != nullptr);
    if(event->type() == ProtocolEvent::getEventType())
        d_ptr->processIncoming(event);
    else if(event->type() == CommandEvent::eventType())
        d_ptr->processOutgoing(event);
    else if(event->type() == NetworkStatusEvent::getEventType()) {
        NetworkStatusEvent *sEvent = static_cast<NetworkStatusEvent *>(event);
        Q_ASSERT(sEvent != nullptr);
        d_ptr->handleNetworkStatusEvent(sEvent);
    }
}
}

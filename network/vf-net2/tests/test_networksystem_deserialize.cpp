#include "test_networksystem_deserialize.h"
#include "vn_networksystem.h"
#include "vn_protocolevent.h"
#include "ecs_schema_generated.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>
#include <flatbuffers/flatbuffers.h>
#include <QUuid>
#include <QTest>
#include <initializer_list>
#include <memory>
#include <vector>

QTEST_MAIN(test_networksystem_deserialize)

using namespace VeinFrameworkIDL;

namespace
{
constexpr int testEntityId = 42;

enum class EventPayload { Present, Missing };

flatbuffers::Offset<ECSEvent> createEvent(flatbuffers::FlatBufferBuilder &builder, EventPayload payload)
{
    flatbuffers::Offset<flatbuffers::String> eventDataOffset; // null offset: field is not written at all
    if(payload == EventPayload::Present) {
        VeinComponent::EntityData entityData;
        entityData.setCommand(VeinComponent::EntityData::Command::ECMD_SUBSCRIBE);
        entityData.setEntityId(testEntityId);
        const QByteArray serialized = entityData.serialize();
        eventDataOffset = builder.CreateString(serialized.constData(), serialized.size());
    }
    return CreateECSEvent(builder, EventSubtype_VC_NOTIFICATION, VeinComponent::EntityData::dataType(), eventDataOffset);
}

QByteArray finishEnvelope(flatbuffers::FlatBufferBuilder &builder, flatbuffers::Offset<ECSEnvelope> envelope)
{
    builder.Finish(envelope);
    return QByteArray(reinterpret_cast<const char *>(builder.GetBufferPointer()), static_cast<int>(builder.GetSize()));
}

QByteArray createEnvelope(std::initializer_list<EventPayload> events)
{
    flatbuffers::FlatBufferBuilder builder;
    std::vector<flatbuffers::Offset<ECSEvent>> eventOffsets;
    for(EventPayload payload : events)
        eventOffsets.push_back(createEvent(builder, payload));
    return finishEnvelope(builder, CreateECSEnvelope(builder, builder.CreateVector(eventOffsets)));
}

QByteArray createEnvelopeWithoutEventVector()
{
    flatbuffers::FlatBufferBuilder builder;
    return finishEnvelope(builder, CreateECSEnvelope(builder)); // ecsEvents is not written at all
}

// All fields in ecs_schema.fbs are optional (none is marked '(required)'), so the
// verifier accepts buffers where they are absent. Tests assert this precondition so
// they cannot pass just because the verifier rejected the buffer.
bool isAcceptedByVerifier(const QByteArray &buffer)
{
    flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t *>(buffer.constData()), buffer.size());
    return VerifyECSEnvelopeBuffer(verifier);
}

std::vector<std::unique_ptr<QEvent>> processRemoteBuffer(const QByteArray &buffer, const QUuid &peerId = QUuid::createUuid())
{
    VeinNet::NetworkSystem networkSystem;
    std::vector<std::unique_ptr<QEvent>> forwarded;
    // Not attached to an event handler: whatever is emitted is ours to delete
    QObject::connect(&networkSystem, &VeinEvent::EventSystem::sigSendEvent, [&](QEvent *event) {
        forwarded.emplace_back(event);
    });
    VeinNet::ProtocolEvent protocolEvent(VeinNet::ProtocolEvent::EventOrigin::EO_REMOTE);
    protocolEvent.setPeerId(peerId);
    protocolEvent.setBuffer(buffer);
    networkSystem.processEvent(&protocolEvent);
    return forwarded;
}
}

void test_networksystem_deserialize::wellFormedEnvelopeIsForwarded()
{
    const QByteArray buffer = createEnvelope({EventPayload::Present});
    QVERIFY(isAcceptedByVerifier(buffer));

    const QUuid peerId = QUuid::createUuid();
    const auto forwarded = processRemoteBuffer(buffer, peerId);

    QCOMPARE(static_cast<int>(forwarded.size()), 1);
    QCOMPARE(static_cast<int>(forwarded[0]->type()), VeinEvent::CommandEvent::getQEventType());
    const auto *commandEvent = static_cast<VeinEvent::CommandEvent *>(forwarded[0].get());
    QCOMPARE(commandEvent->peerId(), peerId);
    QCOMPARE(commandEvent->eventData()->type(), VeinComponent::EntityData::dataType());
    QCOMPARE(commandEvent->eventData()->entityId(), testEntityId);
}

void test_networksystem_deserialize::envelopeWithoutEventVectorIsIgnored()
{
    const QByteArray buffer = createEnvelopeWithoutEventVector();
    QVERIFY(isAcceptedByVerifier(buffer));

    const auto forwarded = processRemoteBuffer(buffer);

    QCOMPARE(static_cast<int>(forwarded.size()), 0);
}

void test_networksystem_deserialize::eventWithoutEventDataIsIgnored()
{
    const QByteArray buffer = createEnvelope({EventPayload::Missing});
    QVERIFY(isAcceptedByVerifier(buffer));

    const auto forwarded = processRemoteBuffer(buffer);

    QCOMPARE(static_cast<int>(forwarded.size()), 0);
}

void test_networksystem_deserialize::eventWithoutEventDataDoesNotDropOtherEvents()
{
    const QByteArray buffer = createEnvelope({EventPayload::Present, EventPayload::Missing, EventPayload::Present});
    QVERIFY(isAcceptedByVerifier(buffer));

    const auto forwarded = processRemoteBuffer(buffer);

    QCOMPARE(static_cast<int>(forwarded.size()), 2);
}

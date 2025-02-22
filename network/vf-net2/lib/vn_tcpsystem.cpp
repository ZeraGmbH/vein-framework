#include "vn_tcpsystem.h"
#include "vn_protocolevent.h"
#include "vn_networkstatusevent.h"
#include <vtcp_peer.h>
#include <vtcp_server.h>

Q_LOGGING_CATEGORY(VEIN_NET_TCP, VEIN_DEBUGNAME_NET_TCP)
Q_LOGGING_CATEGORY(VEIN_NET_TCP_VERBOSE, VEIN_DEBUGNAME_NET_TCP_VERBOSE)

using namespace VeinEvent;

namespace VeinNet
{
TcpSystem::TcpSystem(VeinTcp::AbstractTcpNetworkFactoryPtr tcpNetworkFactory, QObject *t_parent) :
    EventSystem(t_parent),
    m_tcpNetworkFactory(tcpNetworkFactory),
    m_server(new VeinTcp::TcpServer(tcpNetworkFactory, this))
{
    init();
}

void TcpSystem::init()
{
    vCDebug(VEIN_NET_TCP)  << "Created TCP system";
    connect(m_server, &VeinTcp::TcpServer::sigClientConnected, this, &TcpSystem::onClientConnected);
}

TcpSystem::~TcpSystem()
{
    vCDebug(VEIN_NET_TCP)  << "Destroyed TCP system";
}

bool TcpSystem::startServer(quint16 t_port, bool t_systemdSocket)
{
    Q_ASSERT(m_server->isListening() == false);
    return m_server->startServer(t_port, t_systemdSocket);
}

void TcpSystem::connectToServer(const QString &t_host, quint16 t_port)
{
    VF_ASSERT(t_host.isEmpty() == false, "Empty host");
    VF_ASSERT(t_port > 0, "Port must be > 0");
    vCDebug(VEIN_NET_TCP) << "Attempting connection to:"<< t_host << "on port:" << t_port;
    VeinTcp::TcpPeer *tmpPeer = new VeinTcp::TcpPeer(m_tcpNetworkFactory, this);
    connect(tmpPeer, &VeinTcp::TcpPeer::sigSocketError, this, &TcpSystem::onSocketError);
    connect(tmpPeer, &VeinTcp::TcpPeer::sigConnectionEstablished, this, &TcpSystem::onConnectionEstablished);
    connect(tmpPeer, &VeinTcp::TcpPeer::sigConnectionClosed, this, &TcpSystem::onConnectionClosed);
    tmpPeer->startConnection(t_host, t_port);
}

void TcpSystem::onClientConnected(VeinTcp::TcpPeer *peer)
{
    Q_ASSERT(peer != nullptr);
    connect(peer, &VeinTcp::TcpPeer::sigMessageReceived, this, &TcpSystem::onMessageReceived);
    m_waitingAuth.append(peer);
    connect(peer, &VeinTcp::TcpPeer::sigConnectionClosed, this, &TcpSystem::onClientDisconnected);
    peer->sendMessage(QByteArray("welcome")); //message content is irrelevant
}

void TcpSystem::onConnectionEstablished(VeinTcp::TcpPeer *t_peer)
{
    Q_ASSERT(t_peer!=nullptr);
    m_waitingAuth.append(t_peer);
    connect(t_peer, &VeinTcp::TcpPeer::sigMessageReceived, this, &TcpSystem::onMessageReceived);
    t_peer->sendMessage(QByteArray("hello")); //message content is irrelevant
}

void TcpSystem::onConnectionClosed(VeinTcp::TcpPeer *t_peer)
{
    Q_ASSERT(t_peer!=nullptr);
    QUuid tmpPeerId = t_peer->getPeerId();
    vCDebug(VEIN_NET_TCP) << "Disconnected from server with ID:" << tmpPeerId;
    m_waitingAuth.removeAll(t_peer);
    m_peerList.remove(tmpPeerId);
}

void TcpSystem::onClientDisconnected(VeinTcp::TcpPeer *t_peer)
{
    Q_ASSERT(t_peer!=nullptr);
    QUuid tmpPeerId = t_peer->getPeerId();
    NetworkStatusEvent *sEvent = new NetworkStatusEvent(NetworkStatusEvent::NetworkStatus::NSE_DISCONNECTED, tmpPeerId);
    emit sigSendEvent(sEvent);
    vCDebug(VEIN_NET_TCP) << "Client disconnected with ID:" << tmpPeerId << "sent NetworkStatusEvent:" << sEvent;
    m_waitingAuth.removeAll(t_peer);
    m_peerList.remove(tmpPeerId);
    emit sigClientDisconnected(tmpPeerId);
    delete t_peer;
}

void TcpSystem::onMessageReceived(VeinTcp::TcpPeer *t_sender, QByteArray t_buffer)
{
    Q_ASSERT(t_buffer.isNull() == false);
    Q_ASSERT(t_sender!=nullptr);
    //vCDebug(VEIN_NET_TCP_VERBOSE)  << "Message received:" << proto->DebugString().c_str();
    if(m_waitingAuth.contains(t_sender))
    {
        QUuid newId = QUuid::createUuid();
        m_peerList.insert(newId, t_sender);
        vCDebug(VEIN_NET_TCP) << "New connection with id:" << newId;
        t_sender->setPeerId(newId);

        m_waitingAuth.removeAll(t_sender);
        emit sigConnnectionEstablished(newId);
    }
    ProtocolEvent *tmpEvent = new ProtocolEvent(ProtocolEvent::EventOrigin::EO_REMOTE);
    tmpEvent->setBuffer(t_buffer);
    tmpEvent->setPeerId(t_sender->getPeerId());
    emit sigSendEvent(tmpEvent);
}

void TcpSystem::onSocketError(VeinTcp::TcpPeer *t_peer, QAbstractSocket::SocketError t_socketError)
{
    Q_ASSERT(t_peer != nullptr);
    NetworkStatusEvent *sEvent = new NetworkStatusEvent(NetworkStatusEvent::NetworkStatus::NSE_SOCKET_ERROR, t_peer->getPeerId());
    sEvent->setError(t_socketError);
    qCCritical(VEIN_NET_TCP) << "Connection error on network with id:" << t_peer->getPeerId() << "error:" << t_socketError << "sent NetworkStatusEvent:" << sEvent;
    emit sigSendEvent(sEvent);
}

void TcpSystem::processEvent(QEvent *t_event)
{
    Q_ASSERT(t_event != nullptr);
    if(t_event->type()==ProtocolEvent::getQEventType()) {
        ProtocolEvent *pEvent=nullptr;
        pEvent = static_cast<ProtocolEvent *>(t_event);
        Q_ASSERT(pEvent != nullptr);
        /// @todo rework event origin concept
        // do not process protocol events from foreign systems, that is the job of NetworkSystem
        if(pEvent->isOfLocalOrigin() == true) { //event is outgoing
            //send to all
            if(pEvent->receivers().isEmpty()) {
                const auto tmpPeerlist = m_peerList.values();
                vCDebug(VEIN_NET_TCP_VERBOSE) << "Sending ProtocolEvent" << pEvent << "to receivers:" << tmpPeerlist;
                for(VeinTcp::TcpPeer *tmpPeer : tmpPeerlist)
                    tmpPeer->sendMessage(pEvent->buffer());
            }
            else //send to all explicit receivers
            {
                const auto tmpEventReceiversCopy = pEvent->receivers();
                for(const QUuid &receiverId : tmpEventReceiversCopy) {
                    VeinTcp::TcpPeer *tmpPeer = m_peerList.value(receiverId,0);
                    if(tmpPeer) {
                        vCDebug(VEIN_NET_TCP_VERBOSE) << "Sending ProtocolEvent" << pEvent << "to receiver:" << tmpPeer;
                        tmpPeer->sendMessage(pEvent->buffer());
                    }
                }
            }
        }
    }
}
}

#include "vtcp_serverworkermock.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"
#include <systemd/sd-daemon.h>

namespace VeinTcp
{

QHash<quint16 /* port */, TcpServerWorkerMock*> TcpServerWorkerMock::m_serverMocks;

TcpServerWorkerMock::TcpServerWorkerMock(TcpServer *server, secret) :
    m_server(server)
{
}

TcpServerWorkerMock::~TcpServerWorkerMock()
{
    m_serverMocks.remove(m_portListening);
}

bool TcpServerWorkerMock::startServer(quint16 port, bool systemdSocket)
{
    Q_UNUSED(systemdSocket)
    if(!m_serverMocks.contains(port)) {
        m_portListening = port;
        m_serverMocks[port] = this;
        return true;
    }
    // we don't know yet how the production code handles this
    return false;
}

bool TcpServerWorkerMock::isListenActive()
{
    return m_portListening != 0;
}

TcpServerWorkerMock *TcpServerWorkerMock::getServerMock(quint16 port)
{
    TcpServerWorkerMock *server = nullptr;
    if(m_serverMocks.contains(port))
        server = m_serverMocks[port];
    return server;
}

void TcpServerWorkerMock::emitSigClientConnected(TcpPeer *peer)
{
    QMetaObject::invokeMethod(this,
                              "doEmitSigClientConnected",
                              Qt::QueuedConnection,
                              Q_ARG(VeinTcp::TcpPeer*, peer));
}

void TcpServerWorkerMock::doEmitSigClientConnected(TcpPeer *peer)
{
    emit m_server->sigClientConnected(peer);
}

}

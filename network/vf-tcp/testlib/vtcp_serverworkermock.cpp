#include "vtcp_serverworkermock.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"
#include <systemd/sd-daemon.h>

namespace VeinTcp
{

TcpServerWorkerMock::TcpServerWorkerMock(TcpServer *server, secret) :
    m_server(server)
{
}

bool TcpServerWorkerMock::startServer(quint16 port, bool systemdSocket)
{
    m_bListening = true;
    return true;
}

bool TcpServerWorkerMock::isListenActive()
{
    return m_bListening;
}

}

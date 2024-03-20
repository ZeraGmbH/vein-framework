#ifndef VTCP_SERVERWORKERINTERFACE_H
#define VTCP_SERVERWORKERINTERFACE_H

#include "vtcp_peerworkerinterface.h"
#include <QtGlobal>
#include <memory>

namespace VeinTcp
{
class TcpPeer;

class TcpServerWorkerInterface
{
public:
    virtual ~TcpServerWorkerInterface() = default;
    virtual bool startServer(quint16 port, bool systemdSocket) = 0;
    virtual bool isListenActive() = 0;
    virtual TcpPeerWorkerInterfacePtr createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor) = 0;
};

typedef std::unique_ptr<TcpServerWorkerInterface> TcpServerWorkerInterfacePtr;
}

#endif // VTCP_SERVERWORKERINTERFACE_H

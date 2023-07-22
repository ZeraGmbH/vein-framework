#ifndef VTCP_SERVERWORKERINTERFACE_H
#define VTCP_SERVERWORKERINTERFACE_H

#include <QtGlobal>

namespace VeinTcp
{
class TcpServerWorkerInterface
{
public:
    virtual bool startServer(quint16 port, bool systemdSocket) = 0;
    virtual bool isListenActive() = 0;
};
}

#endif // VTCP_SERVERWORKERINTERFACE_H

#ifndef VF_SERVER_RPC_REGISTER_H
#define VF_SERVER_RPC_REGISTER_H

#include <QEvent>
#include <QString>

class VfServerRpcRegister
{
public:
    static QEvent* generateEvent(int entityId, const QString &rpcSignature);
};

#endif // VF_SERVER_RPC_REGISTER_H

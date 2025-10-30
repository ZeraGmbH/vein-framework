#ifndef VF_CPP_RPC_HELPER_H
#define VF_CPP_RPC_HELPER_H

#include <QString>

class VfCppRpcHelper
{
public:
    static QString getRpcTypesFromSignature(const QString &rpcSignature);
    static QStringList getRpcTypesListFromSignature(const QString &rpcSignature);
};

#endif // VF_CPP_RPC_HELPER_H

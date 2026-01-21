#ifndef VF_CPP_RPC_HELPER_H
#define VF_CPP_RPC_HELPER_H

#include <QString>

class VfCppRpcHelper
{
public:
    static QString getRpcTypesFromSignature(const QString &rpcSignature);
    static QStringList getRpcTypesListFromSignature(const QString &rpcSignature);
    static QStringList getRpcParamNamesList(const QString &RPC);
    static QString getRpcName(const QString &RPC);
};

#endif // VF_CPP_RPC_HELPER_H

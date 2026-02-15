#ifndef VF_RPC_PARAMS_PARSER_H
#define VF_RPC_PARAMS_PARSER_H

#include <QString>
#include <QVariantMap>

class VfRpcParamsParser
{
public:
    static QString parseRpcParams(const QString &rpcName, const QVariantMap &params);
};

#endif // VF_RPC_PARAMS_PARSER_H

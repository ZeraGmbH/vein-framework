#include "vf_rpc_params_parser.h"

QString VfRpcParamsParser::parseRpcParams(QString rpcName, QVariantMap params)
{
    QStringList paramStrings;
    for(auto i = params.constBegin(); i != params.constEnd(); i++) {
        QString name = i.key();
        QString type = i.value().typeName();
        paramStrings.append(type + " " + name);
    }
    return rpcName + "(" + paramStrings.join(",") + ")";
}

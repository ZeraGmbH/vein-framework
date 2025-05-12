#include "vf-cpp-rpc-signature.h"

QString VfCpp::VfCppRpcSignature::createRpcSignature(QString rpcName, RPCParams paramDescriptions)
{
    QString signature;
    signature = rpcName;
    signature.append("(");
    const QStringList params = paramDescriptions.keys();
    for(const QString &param : params) {
        signature.append(paramDescriptions[param]);
        signature.append(" ");
        signature.append(param);
        signature.append(",");
    }
    if(signature.at(signature.size()-1) == ",")
        signature.remove(signature.size()-1,1);
    signature.append(")");
    return signature;
}

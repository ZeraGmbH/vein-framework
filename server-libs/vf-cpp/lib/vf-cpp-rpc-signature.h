#ifndef VFCPPRPCSIGNATURE_H
#define VFCPPRPCSIGNATURE_H

#include <QString>
#include <QMap>

namespace VfCpp {

class VfCppRpcSignature
{
public:
    typedef  QMap<QString, QString> RPCParams;
    static QString createRpcSignature(QString rpcName, RPCParams paramDescriptions);
};
}

#endif // VFCPPRPCSIGNATURE_H

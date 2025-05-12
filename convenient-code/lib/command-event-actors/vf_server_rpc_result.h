#ifndef VFSERVERRPCRESULT_H
#define VFSERVERRPCRESULT_H

#include <QVariantMap>
#include <QEvent>
#include <QString>

class VfServerRpcResult
{
public:
    static QEvent* generateEvent(int entityId, QString rpcSignature, QVariantMap returnValue);
};

#endif // VFSERVERRPCRESULT_H

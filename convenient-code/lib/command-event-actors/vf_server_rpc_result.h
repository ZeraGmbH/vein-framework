#ifndef VFSERVERRPCRESULT_H
#define VFSERVERRPCRESULT_H

#include "ve_commandevent.h"
#include <QVariantMap>
#include <QEvent>
#include <QString>

class VfServerRpcResult
{
public:
    static VeinEvent::CommandEvent *generateEvent(int entityId, const QString &rpcSignature, const QVariantMap &returnValue);
};

#endif // VFSERVERRPCRESULT_H

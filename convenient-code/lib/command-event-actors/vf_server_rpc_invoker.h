#ifndef VFSERVERRPCINVOKER_H
#define VFSERVERRPCINVOKER_H

#include "vfrpceventgenerator.h"
#include <memory>
#include <QObject>
#include <QVariantMap>

class VfServerRPCInvoker : public VfRPCEventGenerator
{
    Q_OBJECT
protected:
    QEvent *generateEvent(int entityId, QString procedureName, QVariantMap parameters, QUuid identifier) override;
};
typedef std::shared_ptr<VfServerRPCInvoker> VfServerRPCInvokerPtr;

#endif // VFSERVERRPCINVOKER_H

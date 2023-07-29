#ifndef VFATOMICCLIENTCOMPONENTSETTER_H
#define VFATOMICCLIENTCOMPONENTSETTER_H

#include "vfcmdeventitem.h"
#include <QObject>

class VfAtomicClientComponentSetter : public QObject, public VfCmdEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfAtomicClientComponentSetter> create(int entityId, QString componentName);
    VfAtomicClientComponentSetter(int entityId, QString componentName);
    void startSetComponent(QVariant oldValue, QVariant newValue);
signals:
    void sigSetFinish(bool ok);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void emitSigSetFinish(bool ok);
private:
    QString m_componentName;
};

typedef std::shared_ptr<VfAtomicClientComponentSetter> VfAtomicClientComponentSetterPtr;

#endif // VFATOMICCLIENTCOMPONENTSETTER_H

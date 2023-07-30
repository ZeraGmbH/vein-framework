#ifndef VFATOMICCLIENTCOMPONENTSETTER_H
#define VFATOMICCLIENTCOMPONENTSETTER_H

#include <vfcmdeventitemcomponent.h>
#include <QObject>

class VfAtomicClientComponentSetter : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfAtomicClientComponentSetter> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfAtomicClientComponentSetter(QString componentName, VfCmdEventItemEntityPtr entityItem);

    void startSetComponent(QVariant oldValue, QVariant newValue);
    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComonentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigSetFinish(bool ok);
private:
    void emitSigSetFinish(bool ok);
};

typedef std::shared_ptr<VfAtomicClientComponentSetter> VfAtomicClientComponentSetterPtr;

#endif // VFATOMICCLIENTCOMPONENTSETTER_H

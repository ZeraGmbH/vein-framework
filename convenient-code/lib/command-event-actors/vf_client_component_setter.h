#ifndef VFATOMICCLIENTCOMPONENTSETTER_H
#define VFATOMICCLIENTCOMPONENTSETTER_H

#include <vf_component_event_item.h>
#include <QObject>

class VfClientComponentSetter : public QObject, public VfComponentEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfClientComponentSetter> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfClientComponentSetter(QString componentName, VfCmdEventItemEntityPtr entityItem);

    static QEvent *generateEvent(int entityId, QString componentName, QVariant oldValue, QVariant newValue);
    void startSetComponent(QVariant oldValue, QVariant newValue);
    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigSetFinish(bool ok);
private:
    void emitSigSetFinish(bool ok);
};

typedef std::shared_ptr<VfClientComponentSetter> VfClientComponentSetterPtr;

#endif // VFATOMICCLIENTCOMPONENTSETTER_H

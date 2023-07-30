#ifndef VFCOMPONENTCHANGENOTIFIER_H
#define VFCOMPONENTCHANGENOTIFIER_H

#include <vfcmdeventitemcomponent.h>
#include <memory>

class VfComponentChangeNotifier : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfComponentChangeNotifier> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfComponentChangeNotifier(QString componentName, VfCmdEventItemEntityPtr entityItem);

    const QVariant &getValue() const;
    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComonentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigValueChanged();
private:
    QVariant m_componentValue;
};

typedef std::shared_ptr<VfComponentChangeNotifier> VfSimpleChangeNotifierPtr;

#endif // VFCOMPONENTCHANGENOTIFIER_H

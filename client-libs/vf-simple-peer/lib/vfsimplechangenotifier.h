#ifndef VFSIMPLECHANGENOTIFIER_H
#define VFSIMPLECHANGENOTIFIER_H

#include <vfcmdeventitemcomponent.h>
#include <memory>

class VfSimpleChangeNotifier : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfSimpleChangeNotifier> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfSimpleChangeNotifier(QString componentName, VfCmdEventItemEntityPtr entityItem);
    const QVariant &getValue() const;
signals:
    void sigValueChanged();
private:
    void processComponentEventData(const VeinComponent::ComponentData *cData) override;
    QVariant m_componentValue;
};

typedef std::shared_ptr<VfSimpleChangeNotifier> VfSimpleChangeNotifierPtr;

#endif // VFSIMPLECHANGENOTIFIER_H

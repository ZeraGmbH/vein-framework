#ifndef VFSIMPLECHANGENOTIFIER_H
#define VFSIMPLECHANGENOTIFIER_H

#include <vfcommandeventhandlercomponentitem.h>
#include <memory>

class VfSimpleChangeNotifier : public QObject, public VfCommandEventHandlerComponentItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfSimpleChangeNotifier> create(int entityId, QString componentName);
    VfSimpleChangeNotifier(int entityId, QString componentName);
    const QVariant &getValue() const;
signals:
    void sigValueChanged();
private:
    void processComponentEventData(const VeinComponent::ComponentData *cData) override;
    QVariant m_componentValue;
};

typedef std::shared_ptr<VfSimpleChangeNotifier> VfSimpleChangeNotifierPtr;

#endif // VFSIMPLECHANGENOTIFIER_H

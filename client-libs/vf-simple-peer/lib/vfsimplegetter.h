#ifndef VFSIMPLEGETTER_H
#define VFSIMPLEGETTER_H

#include <vfcommandeventhandlercomponentitem.h>
#include <memory>

class VfSimpleGetter : public QObject, public VfCommandEventHandlerComponentItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfSimpleGetter> create(int entityId, QString componentName);
    VfSimpleGetter(int entityId, QString componentName);
    void startGetComponent();
signals:
    void sigGetFinish(bool ok, QVariant value);
private:
    void processComponentEventData(const VeinComponent::ComponentData *cData) override;
};

typedef std::shared_ptr<VfSimpleGetter> VfSimpleGetterPtr;

#endif // VFSIMPLEGETTER_H

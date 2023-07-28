#ifndef VFSIMPLEGETTER_H
#define VFSIMPLEGETTER_H

#include <vfcmdeventitemcomponent.h>
#include <memory>

class VfSimpleGetter : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfSimpleGetter> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfSimpleGetter(QString componentName, VfCmdEventItemEntityPtr entityItem);
    void startGetComponent();
signals:
    void sigGetFinish(bool ok, QVariant value);
private:
    void processComponentEventData(const VeinComponent::ComponentData *cData) override;
};

typedef std::shared_ptr<VfSimpleGetter> VfSimpleGetterPtr;

#endif // VFSIMPLEGETTER_H

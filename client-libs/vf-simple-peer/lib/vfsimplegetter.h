#ifndef VFSIMPLEGETTER_H
#define VFSIMPLEGETTER_H

#include "vfcommandeventhandleritem.h"
#include <memory>

class VfSimpleGetter : public QObject, public VfCommandEventHandlerItem
{
    Q_OBJECT
public:
    static std::unique_ptr<VfSimpleGetter> create(int entityId, QString componentName);
    VfSimpleGetter(int entityId, QString componentName);
    void startGetComponent();
    QString getComponentName();
signals:
    void sigGetFinish(bool ok, QVariant value);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    QString m_componentName;
};

typedef std::shared_ptr<VfSimpleGetter> VfSimpleGetterPtr;

#endif // VFSIMPLEGETTER_H

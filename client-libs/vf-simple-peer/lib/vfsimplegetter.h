#ifndef VFSIMPLEGETTER_H
#define VFSIMPLEGETTER_H

#include "vfcommandeventhandleritem.h"
#include <memory>

class VfSimpleGetter : public QObject, public VfCommandEventHandlerItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfSimpleGetter> create(int entityId, QStringList componentNames);
    VfSimpleGetter(int entityId, QStringList componentNames);
    void getComponent(QString componentName);
signals:
    void sigGetFinish(int entityId, QString componentName, bool ok, QVariant value);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    QStringList m_componentNames;
};

typedef std::shared_ptr<VfSimpleGetter> VfSimpleGetterPtr;

#endif // VFSIMPLEGETTER_H

#ifndef VFSIMPLESETTER_H
#define VFSIMPLESETTER_H

#include "vfcommandeventhandleritem.h"
#include <QObject>

class VfSimpleSetter : public QObject, public VfCommandEventHandlerItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfSimpleSetter> create(int entityId, QString componentName);
    VfSimpleSetter(int entityId, QString componentName);
    void startSetComponent(QVariant oldValue, QVariant newValue);
signals:
    void sigSetFinish(bool ok);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void emitSigSetFinish(bool ok);
private slots:
    void doEmitSigSetFinish(bool ok);
private:
    QString m_componentName;
};

typedef std::shared_ptr<VfSimpleSetter> VfSimpleSetterPtr;

#endif // VFSIMPLESETTER_H

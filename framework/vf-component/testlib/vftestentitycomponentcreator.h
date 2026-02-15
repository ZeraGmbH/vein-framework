#ifndef VFTESTENTITYCOMPONENTCREATOR_H
#define VFTESTENTITYCOMPONENTCREATOR_H

#include "vftestcomponentdata.h"
#include <ve_eventsystem.h>
#include <QHash>
#include <QSet>

class VfTestEntityComponentCreator : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    bool createEntityComponent(int entityId, const QString &componentName, const QVariant &initialValue = QVariant());
private:
    void createEntity(int entityId);
    void createComponent(int entityId, const QString &componentName, const QVariant &initialValue);
    void processEvent(QEvent *) override;

    QHash<int, QSet<QString>> m_entityComponentsCreated;
};

#endif // VFTESTENTITYCOMPONENTCREATOR_H

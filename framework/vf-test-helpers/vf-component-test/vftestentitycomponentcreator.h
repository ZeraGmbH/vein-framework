#ifndef VFTESTENTITYCOMPONENTCREATOR_H
#define VFTESTENTITYCOMPONENTCREATOR_H

#include "vftestcomponentchangelistener.h"
#include "vftestcomponentdata.h"
#include <ve_eventsystem.h>
#include <QHash>
#include <QSet>

class VfTestEntityComponentCreator : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    bool createEntityComponent(int entityId, QString componentName, QVariant initialValue = QVariant());
private:
    void createEntity(int entityId);
    void createComponent(int entityId, QString componentName, QVariant initialValue);
    bool processEvent(QEvent *) override;

    QHash<int, QSet<QString>> m_entityComponentsCreated;
};

#endif // VFTESTENTITYCOMPONENTCREATOR_H

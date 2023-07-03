#ifndef VFTESTCOMPONENTADDLISTENER_H
#define VFTESTCOMPONENTADDLISTENER_H

#include <ve_eventsystem.h>

class VfTestComponentAddListener : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    typedef QHash<int /* entity id */, QHash<QString /* component name */, QVariant /* component value */>> AddHash;
    void addComponentFilter(int entityId, QString componentName);
    AddHash getComponentHash();
private:
    bool processEvent(QEvent *t_event) override;
    bool matchesComponentFilter(int entityId, QString componentName);

    QHash<int, QSet<QString>> m_componentFilter;
    AddHash m_componentAddHash;
};


#endif // VFTESTCOMPONENTADDLISTENER_H

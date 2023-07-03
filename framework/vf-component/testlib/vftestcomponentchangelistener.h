#ifndef VFTESTCOMPONENTCHANGELISTENER_H
#define VFTESTCOMPONENTCHANGELISTENER_H

#include <ve_eventsystem.h>

class VfTestComponentChangeListener : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    void addComponentFilter(int entityId, QString componentName);
    struct TComponentInfo
    {
        int entityId = -1;
        QString componentName;
        QVariant value;
    };
    QList<TComponentInfo> getComponentChangeList();
private:
    bool processEvent(QEvent *t_event) override;
    bool matchesComponentFilter(int entityId, QString componentName);

    QHash<int, QSet<QString>> m_componentFilter;
    QList<TComponentInfo> m_componentChangeList;
};

#endif // VFTESTCOMPONENTCHANGELISTENER_H

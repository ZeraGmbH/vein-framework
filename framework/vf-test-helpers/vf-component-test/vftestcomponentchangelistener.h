#ifndef VFTESTCOMPONENTCHANGELISTENER_H
#define VFTESTCOMPONENTCHANGELISTENER_H

#include <ve_eventsystem.h>

class VfTestComponentChangeListener : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    void addComponentToListen(int entityId, QString componentName);
    struct TComponentInfo
    {
        int entityId = -1;
        QString componentName;
        QVariant value;
    };
    QList<TComponentInfo> getComponentChangeList();
private:
    bool processEvent(QEvent *t_event) override;
    bool wasComponentInserted(int entityId, QString componentName);

    QHash<int, QSet<QString>> m_hashComponentValuesListening;
    QList<TComponentInfo> m_componentChangeList;
};

#endif // VFTESTCOMPONENTCHANGELISTENER_H

#ifndef VFTESTCOMPONENTCHANGELISTENER_H
#define VFTESTCOMPONENTCHANGELISTENER_H

#include <ve_eventsystem.h>
#include <vs_veinhash.h>

class VfTestComponentChangeListener : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfTestComponentChangeListener(VeinStorage::VeinHash *storageHash);
    void addComponentToListen(QString componentName, const QVariant* componentValue);
    struct TComponentInfo
    {
        int entityId = -1;
        QString componentName;
        QVariant value;
    };
    QList<TComponentInfo> getComponentChangeList();
private:
    bool processEvent(QEvent *t_event) override;

    VeinStorage::VeinHash *m_storageHash;
    QHash <QString, const QVariant*> m_hashComponentValuesListening;
    QList<TComponentInfo> m_componentChangeList;
};

#endif // VFTESTCOMPONENTCHANGELISTENER_H

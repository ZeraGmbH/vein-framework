#ifndef VFTESTCOMPONENTLISTENER_H
#define VFTESTCOMPONENTLISTENER_H

#include <ve_eventsystem.h>
#include <vs_veinhash.h>

class VfTestComponentListener : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfTestComponentListener(VeinStorage::VeinHash *storageHash);
    void addComponentToNotify(QString componentName, const QVariant* componentValue);
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

#endif // VFTESTCOMPONENTLISTENER_H

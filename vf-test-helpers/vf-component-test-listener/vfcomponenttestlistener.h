#ifndef VEINTESTLISTENER_H
#define VEINTESTLISTENER_H

#include <ve_eventsystem.h>
#include <ve_eventhandler.h>
#include <vs_veinhash.h>

class VfComponentTestListener : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfComponentTestListener(int entityId);
    void addComponentToNotify(QString componentName, const QVariant* componentValue);
signals:
    void sigComponentChanged(QString componentName, QVariant newValue);
private:
    void createEntity(int entityId);
    bool processEvent(QEvent *t_event) override;

    VeinEvent::EventHandler m_eventHandler;
    VeinStorage::VeinHash m_storageHash;
    QHash <QString, const QVariant*> m_hashComponentValuesListening;
};

#endif // VEINTESTLISTENER_H

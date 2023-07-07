#ifndef VFTESTCOMPONENTDATA_H
#define VFTESTCOMPONENTDATA_H

#include <ve_eventsystem.h>
#include <QHash>

class VfTestComponentData : public VeinEvent::EventSystem
{
public:
    void setValue(int entityId, QString componentName, QVariant newValue);
    QVariant getValue(int entityId, QString componentName) const;
private:
    void processEvent(QEvent *t_event) override;
    void notifyValueChange(int entityId, QString componentName, QVariant newValue);

    QHash<int, QHash<QString, QVariant>> m_valueHash;
};

#endif // VFTESTCOMPONENTDATA_H

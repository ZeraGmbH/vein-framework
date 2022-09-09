#ifndef VFTESTCOMPONENTDATA_H
#define VFTESTCOMPONENTDATA_H

#include <ve_eventsystem.h>

class VfTestComponentData : public VeinEvent::EventSystem
{
public:
    void setValue(int entityId, QString componentName, QVariant newValue);
    const QVariant &getValue() const;
private:
    bool processEvent(QEvent *t_event) override;
    void notifyValueChange(int entityId, QString componentName);

    QVariant m_value;
};

#endif // VFTESTCOMPONENTDATA_H

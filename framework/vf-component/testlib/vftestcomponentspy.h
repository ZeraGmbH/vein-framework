#ifndef VFTESTCOMPONENTSPY_H
#define VFTESTCOMPONENTSPY_H

#include <vcmp_componentdata.h>
#include <ve_eventsystem.h>

class VfTestComponentSpy : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfTestComponentSpy(VeinComponent::ComponentData::Command cmdToFilter);
    struct TComponentInfo
    {
        int entityId = -1;
        QString componentName;
        QVariant oldValue;
        QVariant newValue;
    };
    QList<TComponentInfo> getComponentChangeList();
private:
    bool processEvent(QEvent *t_event) override;
    VeinComponent::ComponentData::Command m_cmdToFilter;
    QList<TComponentInfo> m_componentChangeList;
};

#endif // VFTESTCOMPONENTSPY_H

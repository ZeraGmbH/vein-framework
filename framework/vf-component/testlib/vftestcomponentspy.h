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
    QList<TComponentInfo> getComponentList() const;
    void reset();
private:
    void processEvent(QEvent *event) override;
    VeinComponent::ComponentData::Command m_cmdToFilter;
    QList<TComponentInfo> m_componentList;
};

#endif // VFTESTCOMPONENTSPY_H

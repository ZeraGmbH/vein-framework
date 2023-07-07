#ifndef VFTESTENTITYSPY_H
#define VFTESTENTITYSPY_H

#include <vcmp_entitydata.h>
#include <ve_eventsystem.h>

class VfTestEntitySpy : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfTestEntitySpy(VeinComponent::EntityData::Command cmdToFilter);
    QList<int> getEntityList() const;
    void reset();
private:
    void processEvent(QEvent *event) override;
    VeinComponent::EntityData::Command m_cmdToFilter;
    QList<int> m_entityList;
};

#endif // VFTESTENTITYSPY_H

#ifndef VFTESTENTITY_H
#define VFTESTENTITY_H

#include <ve_eventsystem.h>

class VfTestEntity : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    void createEntity(int entityId);
private:
    bool processEvent(QEvent *) override;
};

#endif // VFTESTENTITY_H

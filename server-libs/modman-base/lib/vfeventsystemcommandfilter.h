#ifndef VFEVENTSYSTEMCOMMANDFILTER_H
#define VFEVENTSYSTEMCOMMANDFILTER_H

#include <ve_eventsystem.h>
#include <ve_commandevent.h>

class VfEventSystemCommandFilter : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    VfEventSystemCommandFilter(VeinEvent::CommandEvent::EventSubtype subtypeToFilter);
    virtual void processCommandEvent(VeinEvent::CommandEvent *commandEvent) = 0;
    void processEvent(QEvent *event) override final;
private:
    VeinEvent::CommandEvent::EventSubtype m_subtypeToFilter;
    static int m_instanceCount;
};

#endif // VFEVENTSYSTEMCOMMANDFILTER_H

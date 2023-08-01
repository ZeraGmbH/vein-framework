#ifndef VFCOMMANDEVENTHANDLERITEMTEST_H
#define VFCOMMANDEVENTHANDLERITEMTEST_H

#include <vf_cmd_event_item.h>
#include <QObject>

class VfCmdEventHandlerItemTest : public QObject, public VfCmdEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfCmdEventHandlerItemTest> create(int entityId);
    VfCmdEventHandlerItemTest(int entityId);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
signals:
    void sigCommandEventReceived();
    void sigCommandErrorEventReceived();
};

typedef std::shared_ptr<VfCmdEventHandlerItemTest> VfCmdEventHandlerItemTestPtr;

#endif // VFCOMMANDEVENTHANDLERITEMTEST_H

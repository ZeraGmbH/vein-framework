#ifndef VFCOMMANDEVENTHANDLERITEMTEST_H
#define VFCOMMANDEVENTHANDLERITEMTEST_H

#include <vfcmdeventitem.h>
#include <QObject>

class VfCommandEventHandlerItemTest : public QObject, public VfCmdEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfCommandEventHandlerItemTest> create(int entityId);
    VfCommandEventHandlerItemTest(int entityId);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
signals:
    void sigCommandEvenProcessed();
};

typedef std::shared_ptr<VfCommandEventHandlerItemTest> VfCommandEventHandlerItemTestPtr;

#endif // VFCOMMANDEVENTHANDLERITEMTEST_H

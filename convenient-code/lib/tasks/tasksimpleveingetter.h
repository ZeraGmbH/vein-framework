#ifndef TASKSIMPLEVEINGETTER_H
#define TASKSIMPLEVEINGETTER_H

#include "taskcontainersequence.h"
#include "vf_cmd_event_handler_system.h"
#include "vf_cmd_event_item_entity.h"
#include <tasktemplate.h>

class TaskSimpleVeinGetter : public TaskTemplate
{
    Q_OBJECT
public:
    static std::unique_ptr<TaskSimpleVeinGetter> create(int entityId, QString componentName, VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout = 1000);
    TaskSimpleVeinGetter(int entityId, QString componentName, VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout);
    virtual ~TaskSimpleVeinGetter();
    QVariant getValue();
    void start() override;
private:
    std::shared_ptr<QVariant> m_value;
    VfCmdEventItemEntityPtr m_entityItem;
    VfCmdEventHandlerSystemPtr m_cmdEventHandlerSystem;
    TaskContainerSequence m_task;
};

typedef std::unique_ptr<TaskSimpleVeinGetter> TaskSimpleVeinGetterPtr;
#endif // TASKSIMPLEVEINGETTER_H

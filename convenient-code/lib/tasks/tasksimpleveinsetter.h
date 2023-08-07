#ifndef TASKSIMPLEVEINSETTER_H
#define TASKSIMPLEVEINSETTER_H

#include "taskcontainersequence.h"
#include "vf_cmd_event_handler_system.h"
#include "vf_cmd_event_item_entity.h"
#include <tasktemplate.h>

class TaskSimpleVeinSetter : public TaskTemplate
{
    Q_OBJECT
public:
    static std::shared_ptr<TaskSimpleVeinSetter> create(int entityId, QString componentName, QVariant newValue,
                                                 VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout = 1000);
    TaskSimpleVeinSetter(int entityId, QString componentName, QVariant newValue,
                         VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout);
    virtual ~TaskSimpleVeinSetter();
    void start() override;
private:
    std::shared_ptr<QVariant> m_oldValue;
    VfCmdEventItemEntityPtr m_entityItem;
    VfCmdEventHandlerSystemPtr m_cmdEventHandlerSystem;
    TaskContainerSequence m_task;
};

typedef std::shared_ptr<TaskSimpleVeinSetter> TaskSimpleVeinSetterPtr;
#endif // TASKSIMPLEVEINSETTER_H

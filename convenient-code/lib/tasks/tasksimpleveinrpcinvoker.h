#ifndef TASKSIMPLEVEINRPCINVOKER_H
#define TASKSIMPLEVEINRPCINVOKER_H

#include "vf_cmd_event_handler_system.h"
#include <taskcontainersequence.h>
#include <tasktemplate.h>

class TaskSimpleVeinRPCInvoker : public TaskTemplate
{
public:
    static std::unique_ptr<TaskSimpleVeinRPCInvoker> create(int entityId, QString procedureName, QVariantMap parameters,
                                                        VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout = 1000);
    TaskSimpleVeinRPCInvoker(int entityId, QString procedureName, QVariantMap parameters,
                         VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout);
    virtual ~TaskSimpleVeinRPCInvoker();
    void start() override;
private:
    TaskContainerSequence m_task;

};
typedef std::unique_ptr<TaskSimpleVeinRPCInvoker> TaskSimpleVeinRPCInvokerPtr;
#endif // TASKSIMPLEVEINRPCINVOKER_H

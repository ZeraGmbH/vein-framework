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
    QVariant getResult();
private:
    TaskContainerSequence m_task;
    std::shared_ptr<QVariant> m_result;

};
typedef std::unique_ptr<TaskSimpleVeinRPCInvoker> TaskSimpleVeinRPCInvokerPtr;
#endif // TASKSIMPLEVEINRPCINVOKER_H

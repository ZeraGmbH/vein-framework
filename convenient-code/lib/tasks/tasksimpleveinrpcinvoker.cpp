#include "tasksimpleveinrpcinvoker.h"
#include "task_client_entity_subscribe.h"
#include "task_client_rpc_invoker.h"

std::unique_ptr<TaskSimpleVeinRPCInvoker> TaskSimpleVeinRPCInvoker::create(int entityId, QString procedureName, QVariantMap parameters, VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout)
{
    return std::make_unique<TaskSimpleVeinRPCInvoker>(entityId, procedureName, parameters, cmdEventHandlerSystem, timeout);
}

TaskSimpleVeinRPCInvoker::TaskSimpleVeinRPCInvoker(int entityId, QString procedureName, QVariantMap parameters,
                                                   VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout)
{
    std::shared_ptr<QStringList> componentList = std::make_shared<QStringList>();
    m_task.addSub(TaskClientEntitySubscribe::create(entityId, cmdEventHandlerSystem, componentList, timeout));
    m_task.addSub(TaskClientRPCInvoker::create(entityId, procedureName, parameters, cmdEventHandlerSystem, timeout));
    connect(&m_task, &TaskTemplate::sigFinish, this, &TaskTemplate::sigFinish);
}

TaskSimpleVeinRPCInvoker::~TaskSimpleVeinRPCInvoker()
{
}

void TaskSimpleVeinRPCInvoker::start()
{
    m_task.start();
}

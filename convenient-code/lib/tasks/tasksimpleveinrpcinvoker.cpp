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
    m_result = std::make_shared<QVariant>();
    std::shared_ptr<QStringList> componentList = std::make_shared<QStringList>();
    m_task.addSub(TaskClientEntitySubscribe::create(entityId, cmdEventHandlerSystem, componentList, timeout));
    m_task.addSub(TaskClientRPCInvoker::create(entityId, procedureName, parameters, m_result, cmdEventHandlerSystem, timeout));
    connect(&m_task, &TaskTemplate::sigFinish, this, &TaskTemplate::sigFinish);
}

TaskSimpleVeinRPCInvoker::~TaskSimpleVeinRPCInvoker()
{
}

void TaskSimpleVeinRPCInvoker::start()
{
    m_task.start();
}

QVariant TaskSimpleVeinRPCInvoker::getResult()
{
    return *m_result;
}

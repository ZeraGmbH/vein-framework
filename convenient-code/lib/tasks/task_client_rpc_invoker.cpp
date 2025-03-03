#include "task_client_rpc_invoker.h"
#include <taskdecoratortimeout.h>

TaskTemplatePtr TaskClientRPCInvoker::create(int entityId, QString procedureName, QVariantMap parameters, VfCmdEventHandlerSystemPtr commandEventHandler, int timeout, std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskClientRPCInvoker>(entityId, procedureName, parameters, commandEventHandler),
                                             additionalErrorHandler);

}

TaskClientRPCInvoker::TaskClientRPCInvoker(int entityId, QString procedureName, QVariantMap parameters, VfCmdEventHandlerSystemPtr commandEventHandler) :
    m_procedureName(procedureName),
    m_parameters(parameters),
    m_commandEventHandler(commandEventHandler)
{
    m_rpcInvoker = VfClientRPCInvoker::create(entityId);
    m_commandEventHandler->addItem(m_rpcInvoker);
}


TaskClientRPCInvoker::~TaskClientRPCInvoker()
{
    m_commandEventHandler->removeItem(m_rpcInvoker);
}

void TaskClientRPCInvoker::start()
{
    connect(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished, this, [&](bool ok, QUuid identifier, const QVariantMap &resultData) {
        Q_UNUSED(identifier)
        Q_UNUSED(resultData)
        finishTask(ok);
    });
    m_rpcInvoker->invokeRPC(m_procedureName, m_parameters);
}

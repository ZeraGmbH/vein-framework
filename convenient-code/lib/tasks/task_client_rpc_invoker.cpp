#include "task_client_rpc_invoker.h"
#include "vf_client_rpc_invoker.h"
#include <taskdecoratortimeout.h>
#include <vcmp_remoteproceduredata.h>

TaskTemplatePtr TaskClientRPCInvoker::create(int entityId, QString procedureName, QVariantMap parameters, std::shared_ptr<bool> rpcSuccessful,
                                             std::shared_ptr<QVariant> result, std::shared_ptr<QString> errorMsg, VfCmdEventHandlerSystemPtr commandEventHandler, int timeout,
                                             std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskClientRPCInvoker>(entityId, procedureName, parameters, rpcSuccessful, result, errorMsg, commandEventHandler),
                                             additionalErrorHandler);

}

TaskClientRPCInvoker::TaskClientRPCInvoker(int entityId, QString procedureName, QVariantMap parameters, std::shared_ptr<bool> rpcSuccessful,
                                           std::shared_ptr<QVariant> result, std::shared_ptr<QString> errorMsg, VfCmdEventHandlerSystemPtr commandEventHandler) :
    m_commandEventHandler(commandEventHandler),
    m_procedureName(procedureName),
    m_parameters(parameters),
    m_resultData(result),
    m_errorMsg(errorMsg),
    m_rpcSuccessful(rpcSuccessful)
{
    VfClientRPCInvokerPtr client = std::make_unique<VfClientRPCInvoker>();
    m_rpcInvoker = VfRPCInvoker::create(entityId, std::move(client));
    m_commandEventHandler->addItem(m_rpcInvoker);
}

TaskClientRPCInvoker::~TaskClientRPCInvoker()
{
    m_commandEventHandler->removeItem(m_rpcInvoker);
}

void TaskClientRPCInvoker::start()
{
    //calling this multiple times overwrites connections
    m_rpcInvoker->invokeRPC(m_procedureName, m_parameters);
    connect(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished, this, [=](bool ok, QUuid identifier, const QVariantMap &resultData) {
        *m_rpcSuccessful = (resultData[VeinComponent::RemoteProcedureData::s_resultCodeString] == VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_SUCCESS);
        *m_resultData = resultData[VeinComponent::RemoteProcedureData::s_returnString];
        *m_errorMsg = resultData[VeinComponent::RemoteProcedureData::s_errorMessageString].toString();
        finishTask(ok);
    });
}

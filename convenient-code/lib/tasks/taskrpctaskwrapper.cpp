#include "taskrpctaskwrapper.h"

TaskRpcTransactionWrapperPtr TaskRpcTaskWrapper::create(TaskTemplatePtr rpcTransactionTask, const QUuid &rpcCallId)
{
    return std::make_unique<TaskRpcTaskWrapper>(std::move(rpcTransactionTask), rpcCallId);
}

TaskRpcTaskWrapper::TaskRpcTaskWrapper(TaskTemplatePtr rpcTransactionTask, const QUuid &rpcCallId) :
    m_transactionTask(std::move(rpcTransactionTask)),
    m_rpcCallId(rpcCallId)
{
    connect(m_transactionTask.get(), &TaskTemplate::sigFinish,
            this, &TaskRpcTaskWrapper::onFinished);
}

void TaskRpcTaskWrapper::start()
{
    m_transactionTask->start();
}

void TaskRpcTaskWrapper::onFinished(bool ok)
{
    emit sigRpcFinished(ok, m_rpcCallId);
    finishTaskQueued(ok);
}

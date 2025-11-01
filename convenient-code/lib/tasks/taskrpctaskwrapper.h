#ifndef TASK_RPC_TASK_WRAPPER_H
#define TASK_RPC_TASK_WRAPPER_H

#include <tasktemplate.h>
#include <QUuid>

class TaskRpcTaskWrapper;
typedef std::unique_ptr<TaskRpcTaskWrapper> TaskRpcTransactionWrapperPtr;

class TaskRpcTaskWrapper : public TaskTemplate
{
    Q_OBJECT
public:
    static TaskRpcTransactionWrapperPtr create(TaskTemplatePtr rpcTransactionTask, const QUuid &rpcCallId);
    TaskRpcTaskWrapper(TaskTemplatePtr rpcTransactionTask, const QUuid &rpcCallId);
    void start() override;
signals:
    void sigRpcFinished(bool ok, const QUuid &rpcCallId);
private slots:
    void onFinished(bool ok);
private:
    TaskTemplatePtr m_transactionTask;
    QUuid m_rpcCallId;
};


#endif // TASK_RPC_TASK_WRAPPER_H

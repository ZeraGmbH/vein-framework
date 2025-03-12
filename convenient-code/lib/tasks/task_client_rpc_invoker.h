#ifndef TASK_CLIENT_RPC_INVOKER_H
#define TASK_CLIENT_RPC_INVOKER_H

#include "vf_client_rpc_invoker.h"
#include "vf_cmd_event_handler_system.h"
#include <tasktemplate.h>
#include <QObject>

class TaskClientRPCInvoker : public TaskTemplate
{
public:
    static TaskTemplatePtr create(int entityId, QString procedureName, QVariantMap parameters, std::shared_ptr<QVariant> result, VfCmdEventHandlerSystemPtr commandEventHandler,
                                  int timeout, std::function<void()> additionalErrorHandler = []{});
    TaskClientRPCInvoker(int entityId, QString procedureName, QVariantMap parameters, std::shared_ptr<QVariant> result, VfCmdEventHandlerSystemPtr commandEventHandler);
    ~TaskClientRPCInvoker();
    void start() override;
private:
    VfCmdEventHandlerSystemPtr m_commandEventHandler;
    VfClientRPCInvokerPtr m_rpcInvoker;
    QString m_procedureName;
    QVariantMap m_parameters;
    std::shared_ptr<QVariant> m_resultData;
};

#endif // TASK_CLIENT_RPC_INVOKER_H

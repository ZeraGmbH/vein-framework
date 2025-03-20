#ifndef TEST_TASK_CLIENT_RPC_INVOKER_H
#define TEST_TASK_CLIENT_RPC_INVOKER_H

#include <QObject>

class test_task_client_rpc_invoker : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void invokeSubscribedRPC();
    void invokeSubscribedRPCTwice();
    void timeout();
};

#endif // TEST_TASK_CLIENT_RPC_INVOKER_H

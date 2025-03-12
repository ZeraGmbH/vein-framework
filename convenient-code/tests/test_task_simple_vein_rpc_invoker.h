#ifndef TEST_TASK_SIMPLE_VEIN_RPC_INVOKER_H
#define TEST_TASK_SIMPLE_VEIN_RPC_INVOKER_H

#include <QObject>

class test_task_simple_vein_rpc_invoker : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void getValid();
    void getInvalid();
    void getTimeout();
};

#endif // TEST_TASK_SIMPLE_VEIN_RPC_INVOKER_H

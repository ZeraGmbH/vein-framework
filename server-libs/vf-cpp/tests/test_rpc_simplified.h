#ifndef TEST_RPC_SIMPLIFIED_H
#define TEST_RPC_SIMPLIFIED_H

#include <QObject>

class test_rpc_simplified : public QObject
{
    Q_OBJECT
private slots:
    void callRpcValidParam();
    void callRpcInvalidParamValue();
    void callRpcInvalidParamName();
    void callRpcMissingParam();
    void callInvalidRpc();
};

#endif // TEST_RPC_SIMPLIFIED_H

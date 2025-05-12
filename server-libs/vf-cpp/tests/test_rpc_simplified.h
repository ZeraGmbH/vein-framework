#ifndef TEST_RPC_SIMPLIFIED_H
#define TEST_RPC_SIMPLIFIED_H

#include "testveinserverwithmocknet.h"
#include "vftestrpcsimplified.h"
#include <QObject>

class test_rpc_simplified : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void callRpcValidParam();
    void callRpcInvalidParamValue();
    void callRpcInvalidParamName();
    void callRpcMissingParam();
    void callInvalidRpc();
private:
    std::unique_ptr<TestVeinServerWithMockNet> m_serverNet;
    std::unique_ptr<VfTestRpcSimplified> m_additionalEntityWithRpc;
};

#endif // TEST_RPC_SIMPLIFIED_H

#ifndef TEST_RPC_SIMPLIFIED_H
#define TEST_RPC_SIMPLIFIED_H

#include "testveinserverwithmocknet.h"
#include "vfentitywithrpcsimplified.h"
#include "vf_core_stack_client.h"
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
    void callRPCTwice();
    void callRPCRespondingAfterDelay();
private:
    std::unique_ptr<TestVeinServerWithMockNet> m_serverNet;
    std::unique_ptr<VfEntityWithRpcSimplified> m_additionalEntityWithRpc;
    std::unique_ptr<VfCoreStackClient> m_clientStack;
    VfClientRPCInvokerPtr m_rpcInvoker;
};

#endif // TEST_RPC_SIMPLIFIED_H

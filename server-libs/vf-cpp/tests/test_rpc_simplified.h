#ifndef TEST_RPC_SIMPLIFIED_H
#define TEST_RPC_SIMPLIFIED_H

#include "testveinserverwithmocknet.h"
#include "vfentitywithrpcsimplified.h"
#include "vf_core_stack_client.h"
#include <QObject>
#include <QJsonObject>

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
    void invokeRpc(QString rpcName, QString paramName, QVariant paramValue);
    bool isRpcFound(QList<QVariant> spyArguments);
    QVariant getReturnResult(QList<QVariant> spyArguments);
    QVariant getReturnError(QList<QVariant> spyArguments);
    void setupSpy(QJsonObject &jsonEvents);

    std::unique_ptr<TestVeinServerWithMockNet> m_serverNet;
    std::unique_ptr<VfEntityWithRpcSimplified> m_additionalEntityWithRpc;
    std::unique_ptr<VfCoreStackClient> m_clientStack;
    VfRPCInvokerPtr m_rpcInvoker;
    QJsonObject m_veinEventDump;
    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyBottom;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyBottom;
};

#endif // TEST_RPC_SIMPLIFIED_H

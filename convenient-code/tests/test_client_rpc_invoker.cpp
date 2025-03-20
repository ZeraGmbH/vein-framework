#include "test_client_rpc_invoker.h"
#include "testveinserverwithmocknet.h"
#include "vf_client_rpc_invoker.h"
#include "testveinserver.h"
#include "vf_cmd_event_handler_system.h"
#include "vf_core_stack_client.h"
#include "vf_entity_rpc_event_handler.h"
#include "mocktcpnetworkfactory.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_rpc_invoker)

static constexpr int rpcHandlerId = 99999;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    VeinEvent::EventHandler eventHandler;
    TestVeinServer server;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet()
    {
        server.appendEventSystem(&cmdEventHandlerSystem);
    }
};

void test_client_rpc_invoker::errorSignalFromUnsubscribedEntityInvalidRPCNoNet()
{
    ServerNoNet server;
    TimeMachineObject::feedEventLoop();
    QList<int> entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    //vfEntityRpcEventHandler is an entity based on VfCpp
    //We append it to our server as a rpc-event handler
    vfEntityRpcEventHandler rpcEventHandler;
    server.server.appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 2);

    VfClientRPCInvokerPtr invoker = VfClientRPCInvoker::create(rpcHandlerId);
    server.cmdEventHandlerSystem.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_foo(bool p_param)",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), false);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap["RemoteProcedureData::Return"];
    QCOMPARE(resultData, QVariant());
}

void test_client_rpc_invoker::unsubscribedEntityValidRPCNoNet()
{
    ServerNoNet server;
    TimeMachineObject::feedEventLoop();
    QList<int> entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    vfEntityRpcEventHandler rpcEventHandler;
    server.server.appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 2);

    VfClientRPCInvokerPtr invoker = VfClientRPCInvoker::create(rpcHandlerId);
    server.cmdEventHandlerSystem.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap["RemoteProcedureData::Return"];
    QCOMPARE(resultData, false);
}

void test_client_rpc_invoker::unsubscribedEntityValidRPCNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    //vfEntityRpcEventHandler is an entity based on VfCpp
    //We append it to our server as a rpc-event handler
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    QList<int> entities = serverNet.getServer()->getEntityAddList();
    QCOMPARE(entities.size(), 2);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfClientRPCInvokerPtr invoker = VfClientRPCInvoker::create(rpcHandlerId);
    clientStack.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 0);
}

void test_client_rpc_invoker::subscribedEntityValidRPCNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    QList<int> entities = serverNet.getServer()->getEntityAddList();
    QCOMPARE(entities.size(), 2);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    clientStack.subscribeEntity(rpcHandlerId);
    TimeMachineObject::feedEventLoop();

    VfClientRPCInvokerPtr invoker = VfClientRPCInvoker::create(rpcHandlerId);
    clientStack.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap["RemoteProcedureData::Return"];
    QCOMPARE(resultData, false);
}

void test_client_rpc_invoker::subscribedEntityValidRPCTwoInvokers()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    QList<int> entities = serverNet.getServer()->getEntityAddList();
    QCOMPARE(entities.size(), 2);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    clientStack.subscribeEntity(rpcHandlerId);
    TimeMachineObject::feedEventLoop();

    VfClientRPCInvokerPtr invokerOne = VfClientRPCInvoker::create(rpcHandlerId);
    clientStack.addItem(invokerOne);

    QSignalSpy invokerSpyOne(invokerOne.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    QUuid idOne = invokerOne->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    VfClientRPCInvokerPtr invokerTwo = VfClientRPCInvoker::create(rpcHandlerId);
    clientStack.addItem(invokerTwo);

    QSignalSpy invokerSpyTwo(invokerTwo.get(), &VfClientRPCInvoker::sigRPCFinished);
    QUuid idTwo = invokerTwo->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpyOne.count(), 1);
    QCOMPARE(invokerSpyOne[0][1], idOne);

    QCOMPARE(invokerSpyTwo.count(), 1);
    QCOMPARE(invokerSpyTwo[0][1], idTwo);
}

void test_client_rpc_invoker::subscribedEntityNonExistentRPCNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    QList<int> entities = serverNet.getServer()->getEntityAddList();
    QCOMPARE(entities.size(), 2);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    clientStack.subscribeEntity(rpcHandlerId);
    TimeMachineObject::feedEventLoop();

    VfClientRPCInvokerPtr invoker = VfClientRPCInvoker::create(rpcHandlerId);
    clientStack.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_foo(bool p_param)",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 0);
}

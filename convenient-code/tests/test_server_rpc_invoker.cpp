#include "test_server_rpc_invoker.h"
#include "testveinserver.h"
#include "testveinserverwithmocknet.h"
#include "ve_eventhandler.h"
#include "vf_entity_rpc_event_handler.h"
#include "vf_server_rpc_invoker.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_server_rpc_invoker)

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

void test_server_rpc_invoker::invalidRPCNoNet()
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

    VfRPCInvokerPtr invoker = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    server.cmdEventHandlerSystem.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_foo(bool p_param)",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), false); //no signal detected
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, QVariant());
}

void test_server_rpc_invoker::invalidRPCNoNetNoCrossTalk()
{
    ServerNoNet server;
    TimeMachineObject::feedEventLoop();
    QList<int> entities = server.server.getEntityAddList();

    vfEntityRpcEventHandler rpcEventHandler;
    server.server.appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr invoker1 = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    VfRPCInvokerPtr invoker2 = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    server.cmdEventHandlerSystem.addItem(invoker1);
    server.cmdEventHandlerSystem.addItem(invoker2);

    QSignalSpy invokerSpy1(invoker1.get(), &VfRPCInvoker::sigRPCFinished);
    QSignalSpy invokerSpy2(invoker2.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker1->invokeRPC("RPC_foo(bool p_param)", parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy1.count(), 1);
    QList<QVariant> arguments = invokerSpy1[0];
    QCOMPARE(arguments.at(0), false);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, QVariant());
    QCOMPARE(invokerSpy2.count(), 0);
}

void test_server_rpc_invoker::validRPCNoNet()
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

    VfRPCInvokerPtr invoker = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    server.cmdEventHandlerSystem.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, false);
}

void test_server_rpc_invoker::invalidRPCNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr invoker = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_foo",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), false);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, QVariant());
}

void test_server_rpc_invoker::validRPCNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr invoker = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, false);
}

void test_server_rpc_invoker::validRPCTwice()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr invoker = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker->invokeRPC("RPC_forTest",parameters);
    invoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy.count(), 2);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, false);
}

void test_server_rpc_invoker::noCrossTalk()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr invoker1 = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    VfRPCInvokerPtr invoker2 = VfRPCInvoker::create(rpcHandlerId, std::make_unique<VfServerRPCInvoker>());
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker1);
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker2);

    QSignalSpy invokerSpy1(invoker1.get(), &VfRPCInvoker::sigRPCFinished);
    QSignalSpy invokerSpy2(invoker2.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    invoker1->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(invokerSpy1.count(), 1);
    QCOMPARE(invokerSpy2.count(), 0);
}

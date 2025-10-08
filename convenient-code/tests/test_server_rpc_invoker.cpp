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

    VfServerRPCInvokerPtr invoker = VfServerRPCInvoker::create(rpcHandlerId);
    server.cmdEventHandlerSystem.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfServerRPCInvoker::sigRPCFinished);
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

    VfServerRPCInvokerPtr invoker = VfServerRPCInvoker::create(rpcHandlerId);
    server.cmdEventHandlerSystem.addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfServerRPCInvoker::sigRPCFinished);
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

    VfServerRPCInvokerPtr invoker = VfServerRPCInvoker::create(rpcHandlerId);
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfServerRPCInvoker::sigRPCFinished);
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

    VfServerRPCInvokerPtr invoker = VfServerRPCInvoker::create(rpcHandlerId);
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfServerRPCInvoker::sigRPCFinished);
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

    VfServerRPCInvokerPtr invoker = VfServerRPCInvoker::create(rpcHandlerId);
    serverNet.getServer()->getCmdEventHandlerSystem()->addItem(invoker);

    QSignalSpy invokerSpy(invoker.get(), &VfServerRPCInvoker::sigRPCFinished);
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



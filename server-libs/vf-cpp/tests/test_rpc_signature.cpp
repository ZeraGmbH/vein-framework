#include "test_rpc_signature.h"
#include "vf-cpp-rpc-signature.h"
#include <QTest>

QTEST_MAIN(test_rpc_signature)

void test_rpc_signature::singleParam()
{
    VfCpp::VfCppRpcSignature::RPCParams params;
    params.insert("FirstParam", "bool");
    QString rpcName("RPC_TestRPC");
    QCOMPARE(VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, params), "RPC_TestRPC(bool FirstParam)");
}

void test_rpc_signature::twoParams()
{
    VfCpp::VfCppRpcSignature::RPCParams params;
    params.insert("FirstParam", "bool");
    params.insert("SecondParam", "double");
    QString rpcName("RPC_TestRPC");
    QCOMPARE(VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, params), "RPC_TestRPC(bool FirstParam,double SecondParam)");
}

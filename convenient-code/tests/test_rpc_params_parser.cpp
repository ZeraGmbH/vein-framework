#include "test_rpc_params_parser.h"
#include "vf_rpc_params_parser.h"
#include "vf-cpp-rpc-signature.h"
#include <QTest>

QTEST_MAIN(test_rpc_params_parser)

void test_rpc_params_parser::parseNoParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, QMap<QString, QString>());
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseIntSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = 1;

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["firstParam"] = "int";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseBoolSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = true;

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["firstParam"] = "bool";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseStringSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = "foo";

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["firstParam"] = "QString";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseStringListSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = QStringList() << "foo" << "bar";

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["firstParam"] = "QStringList";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseDoubleNumberSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = static_cast<double>(1.11);

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["firstParam"] = "double";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseFloatNumberSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = static_cast<float>(1.11);

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["firstParam"] = "float";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseBoolBoolParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["z"] = true;
    params["a"] = true;

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["z"] = "bool";
    rpcParams["a"] = "bool";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseIntBoolParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["a"] = 1;
    params["z"] = true;

    VfCpp::VfCppRpcSignature::RPCParams rpcParams;
    rpcParams["a"] = "int";
    rpcParams["z"] = "bool";
    QString rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, rpcParams);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

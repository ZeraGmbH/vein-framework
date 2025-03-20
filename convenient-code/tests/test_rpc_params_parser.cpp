#include "test_rpc_params_parser.h"
#include "vf_rpc_params_parser.h"
#include "vf-cpp-rpc.h"
#include <QTest>

QTEST_MAIN(test_rpc_params_parser)

void test_rpc_params_parser::parseNoParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, QMap<QString, QString>());
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseIntSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = 1;

    QMap<QString, QString> signature;
    signature["firstParam"] = "int";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseBoolSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = true;

    QMap<QString, QString> signature;
    signature["firstParam"] = "bool";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseStringSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = "foo";

    QMap<QString, QString> signature;
    signature["firstParam"] = "QString";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseStringListSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = QStringList() << "foo" << "bar";

    QMap<QString, QString> signature;
    signature["firstParam"] = "QStringList";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseDoubleNumberSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = static_cast<double>(1.11);

    QMap<QString, QString> signature;
    signature["firstParam"] = "double";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseFloatNumberSingleParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["firstParam"] = static_cast<float>(1.11);

    QMap<QString, QString> signature;
    signature["firstParam"] = "float";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseBoolBoolParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["z"] = true;
    params["a"] = true;

    QMap<QString, QString> signature;
    signature["z"] = "bool";
    signature["a"] = "bool";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

void test_rpc_params_parser::parseIntBoolParam()
{
    QString rpcName = "RPC_test";
    QVariantMap params;
    params["a"] = 1;
    params["z"] = true;

    QMap<QString, QString> signature;
    signature["a"] = "int";
    signature["z"] = "bool";
    QString rpcSignature = VfCpp::cVeinModuleRpc::createRpcSignature(rpcName, signature);
    QCOMPARE(VfRpcParamsParser::parseRpcParams(rpcName, params), rpcSignature);
}

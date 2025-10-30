#include "vf_entity_rpc_event_handler.h"
#include "vf-cpp-rpc-signature.h"
#include "vf-cpp-rpc-helper.h"
#include <QJsonDocument>

vfEntityRpcEventHandler::vfEntityRpcEventHandler(int id)
{
    m_entity = new VfCpp::VfCppEntity(id);
    m_isInitalized = false;
}

vfEntityRpcEventHandler::~vfEntityRpcEventHandler()
{
    delete m_entity;
}

bool vfEntityRpcEventHandler::initOnce()
{
    if(!m_isInitalized) {
        m_isInitalized = true;
        m_entity->initModule();
        m_entity->createComponent("EntityName", "RPCEventHandler", true);
        m_entity->createRpc(this, "RPC_forTest", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "bool"}}), false);
        m_rpcWithParams = m_entity->createRpc(this, "RpcReturnText", VfCpp::VfCppRpcSignature::RPCParams({{"p_input", "QString"},{"p_readOnly", "bool"}}), false);
        m_entity->createComponent("INF_ModuleInterface", setModuleInterface());
    }
    return true;
}

VfCpp::VfCppEntity *vfEntityRpcEventHandler::getVeinEntity() const
{
    return m_entity;
}

QVariant vfEntityRpcEventHandler::RPC_forTest(QVariantMap parameters)
{
    bool paramBool = parameters["p_param"].toBool();
    return !paramBool;
}

QVariant vfEntityRpcEventHandler::RpcReturnText(QVariantMap parameters)
{
    QString input= parameters["p_input"].toString();
    bool readOnly = parameters["p_readOnly"].toBool();
    if(readOnly)
        return input;
    else
        return "readwrite " + input;
}

QByteArray vfEntityRpcEventHandler::setModuleInterface()
{
    // Add only what's needed for now
    QJsonArray jsonArr;
    QJsonArray jsonSCPIArr;
    QJsonObject jsonObj;
    jsonSCPIArr.append("CALCULATE");
    jsonSCPIArr.append("RPC1");
    jsonSCPIArr.append("2");
    jsonSCPIArr.append("RPC_forTest(bool p_param)");
    jsonSCPIArr.append("0");

    jsonArr.append(jsonSCPIArr);

    jsonSCPIArr = QJsonArray();
    jsonSCPIArr.append("CALCULATE");
    jsonSCPIArr.append("RPC2");
    jsonSCPIArr.append("2");
    jsonSCPIArr.append(m_rpcWithParams->rpcName());
    jsonSCPIArr.append("0");
    jsonSCPIArr.append(VfCppRpcHelper::getRpcTypesFromSignature(m_rpcWithParams->rpcName()));
    jsonArr.append(jsonSCPIArr);

    QJsonObject jsonObj4;
    jsonObj4.insert("RPC", jsonArr);

    jsonObj.insert("SCPIInfo", jsonObj4);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    QByteArray ba;
    ba = jsonDoc.toJson();
    return ba;
}

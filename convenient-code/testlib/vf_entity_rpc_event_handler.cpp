#include "vf_entity_rpc_event_handler.h"
#include "vf-cpp-rpc-signature.h"
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
        m_entity->createComponent("INF_ModuleInterface", setModuleInterface());
        m_entity->createRpc(this, "RPC_forTest", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "bool"}}), false);
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

QByteArray vfEntityRpcEventHandler::setModuleInterface()
{
    // Add only what's needed for now
    QJsonArray jsonArr;
    QJsonArray jsonSCPIArr;
    QJsonObject jsonObj;
    jsonSCPIArr.append("CALCULATE");
    jsonSCPIArr.append("RPC_forTest(p_param)");
    jsonSCPIArr.append("2");
    jsonSCPIArr.append("");
    jsonSCPIArr.append("0");
    jsonSCPIArr.append("bool");

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

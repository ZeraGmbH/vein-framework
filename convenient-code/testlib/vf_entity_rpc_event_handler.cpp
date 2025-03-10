#include "vf_entity_rpc_event_handler.h"

vfEntityRpcEventHandler::vfEntityRpcEventHandler(QObject *parent, int id) : QObject(parent)
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
        m_entity->createRpc(this, "RPC_forTest", VfCpp::cVeinModuleRpc::Param({{"p_param", "bool"}}), false);
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

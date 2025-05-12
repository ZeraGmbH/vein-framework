#include "vf-cpp-rpc-simplified.h"
#include "vf_server_rpc_register.h"
#include "vf_server_rpc_result.h"
#include "vcmp_remoteproceduredata.h"

using namespace VfCpp;

VfCppRpcSimplified::VfCppRpcSimplified(QObject *object, VeinEvent::EventSystem *eventsystem, int entityId, QString rpcName, VfCppRpcSignature::RPCParams parameters) :
    m_object(object),
    m_eventSystem(eventsystem),
    m_entityId(entityId),
    m_rpcName(rpcName)
{
    m_rpcSignature = VfCppRpcSignature::createRpcSignature(m_rpcName, parameters);
    emit m_eventSystem->sigSendEvent(VfServerRpcRegister::generateEvent(m_entityId, m_rpcSignature));
}

QString VfCppRpcSimplified::getSignature()
{
    return m_rpcSignature;
}

void VfCppRpcSimplified::callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters)
{
    QVariantMap params = parameters.value(VeinComponent::RemoteProcedureData::s_parameterString).toMap();
    params.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
    QMetaObject::invokeMethod(m_object, m_rpcName.toUtf8(), Qt::DirectConnection, Q_ARG(QVariantMap, params));
}

void VfCppRpcSimplified::sendRpcResult(const QUuid &callId, QVariant result)
{
    QVariantMap returnVal;
    returnVal.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
    returnVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_SUCCESS);
    if(!result.isNull()) {
        returnVal.insert("RemoteProcedureData::Return", result);
    }
    emit m_eventSystem->sigSendEvent(VfServerRpcResult::generateEvent(m_entityId, m_rpcSignature, returnVal));
}

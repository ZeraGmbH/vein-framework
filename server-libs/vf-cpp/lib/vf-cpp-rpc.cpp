#include "vf-cpp-rpc.h"
#include "vf-cpp-rpc-signature.h"
#include "vf_server_rpc_result.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QRegularExpression>

using namespace VfCpp;

cVeinModuleRpc::cVeinModuleRpc(int entityId,
                               VeinEvent::EventSystem *eventsystem,
                               QObject *p_object,
                               QString p_funcName,
                               QMap<QString,QString> p_parameter,
                               bool p_threaded,
                               bool pFunctionBlocks) :
    m_object(p_object),
    m_function(p_funcName),
    m_parameter(p_parameter),
    m_nEntityId(entityId),
    m_pEventSystem(eventsystem),
    m_threaded(p_threaded),
    m_functionBlocks(pFunctionBlocks)
{
    if(p_threaded && !pFunctionBlocks) {
        qFatal("Do not use unblocking functions with threading - signal won't reach their slots!!");
    }
    m_rpcName = VfCppRpcSignature::createRpcSignature(p_funcName, p_parameter);

    VeinComponent::RemoteProcedureData *rpcData = new VeinComponent::RemoteProcedureData();
    rpcData->setEntityId(m_nEntityId);
    rpcData->setCommand(VeinComponent::RemoteProcedureData::Command::RPCMD_REGISTER);
    rpcData->setProcedureName(m_rpcName);
    rpcData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    rpcData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);

    QObject::connect(this,&cVeinModuleRpc::callFunctionPrivateSignal,this,&cVeinModuleRpc::callFunctionPrivate,Qt::QueuedConnection);

    emit  m_pEventSystem->sigSendEvent(new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, rpcData));
}

cVeinModuleRpc::~cVeinModuleRpc()
{
}

QString cVeinModuleRpc::rpcName() const
{
    return m_rpcName;
}

void cVeinModuleRpc::callFunction(const QUuid &p_callId, const QUuid &p_peerId, const QVariantMap &t_rpcParameters)
{
    emit callFunctionPrivateSignal(p_callId, p_peerId, t_rpcParameters);
}

void cVeinModuleRpc::callFunctionPrivate(const QUuid p_callId, const QUuid p_peerId, const QVariantMap t_rpcParameters)
{
    const auto rpcHandling = [=]() {
        QMutexLocker locker(&(this->m_mutex));

        // check parameters
        QStringList requiredParamList = m_parameter.keys();
        QSet<QString> requiredParamKeys(requiredParamList.begin(), requiredParamList.end());
        const QVariantMap searchParameters = t_rpcParameters.value(VeinComponent::RemoteProcedureData::s_parameterString).toMap();
        QStringList searchParamList = searchParameters.keys();
        requiredParamKeys.subtract(QSet<QString>(searchParamList.begin(), searchParamList.end()));

        bool callOk = false;
        VeinComponent::RemoteProcedureData::RPCResultCodes resultCode = VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_EINVAL;
        QString errorMsg;
        QVariant returnedResult;
        if(requiredParamKeys.isEmpty()) {
            // call actual function
            QVariantMap rpcParameters = searchParameters;
            rpcParameters.insert(VeinComponent::RemoteProcedureData::s_callIdString, p_callId);
            callOk = QMetaObject::invokeMethod(m_object,m_function.toUtf8(),Qt::DirectConnection,
                                                 Q_RETURN_ARG(QVariant, returnedResult),
                                                 Q_ARG(QVariantMap,rpcParameters)
                                                 );
            if(callOk) {
                resultCode = VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_SUCCESS;
            } else {
                resultCode = VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_EINVAL;
                errorMsg = QStringLiteral("Function not implemented");
            }
        }
        else {
            // write error msg on error
            returnedResult = t_rpcParameters;
            resultCode = VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_EINVAL;
            errorMsg = QString("Missing required parameters: [%1]").arg(requiredParamList.join(','));
        }
        m_pendingRpcHash[p_callId] = p_peerId;
        if(!callOk || m_functionBlocks) {
            // send answer now
            sendRpcResult(p_callId, resultCode, errorMsg, returnedResult);
        }
    };

    if(m_threaded) {
        QtConcurrent::run(rpcHandling);
    } else {
        rpcHandling();
    }
}

void cVeinModuleRpc::sendRpcResult(const QUuid &p_callId, VeinComponent::RemoteProcedureData::RPCResultCodes resultCode, QString errorMsg, QVariant returnedResult)
{
    QHash<QUuid, QUuid>::iterator iter = m_pendingRpcHash.find(p_callId);
    if(iter != m_pendingRpcHash.end()) {
        QVariantMap returnVal;
        returnVal.insert(VeinComponent::RemoteProcedureData::s_callIdString, p_callId);
        returnVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, resultCode);
        if(!errorMsg.isEmpty()) {
            returnVal.insert(VeinComponent::RemoteProcedureData::s_errorMessageString, errorMsg);
        }
        if(!returnedResult.isNull()) {
            returnVal.insert(VeinComponent::RemoteProcedureData::s_returnString, returnedResult);
        }

        VeinEvent::CommandEvent *rpcResultEvent = VfServerRpcResult::generateEvent(m_nEntityId, m_rpcName, returnVal);

        auto peerId = iter.value();
        rpcResultEvent->setPeerId(peerId);
        m_pendingRpcHash.erase(iter);
        emit m_pEventSystem->sigSendEvent(rpcResultEvent);
    }
}

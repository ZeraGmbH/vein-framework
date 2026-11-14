#include "vf-cpp-rpc.h"
#include "vf-cpp-rpc-signature.h"
#include "vf_server_rpc_result.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QRegularExpression>

using namespace VfCpp;

cVeinModuleRpc::cVeinModuleRpc(int entityId,
                               VeinEvent::EventSystem *eventsystem,
                               QObject *object,
                               const QString &funcName,
                               const QMap<QString, QString> &parameter,
                               bool threaded,
                               bool functionBlocks) :
    m_object(object),
    m_function(funcName),
    m_parameter(parameter),
    m_nEntityId(entityId),
    m_pEventSystem(eventsystem),
    m_threaded(threaded),
    m_functionBlocks(functionBlocks)
{
    if(threaded && !functionBlocks) {
        qFatal("Do not use unblocking functions with threading - signal won't reach their slots!!");
    }
    m_rpcName = VfCppRpcSignature::createRpcSignature(funcName, parameter);

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

const QString &cVeinModuleRpc::rpcName() const
{
    return m_rpcName;
}

void cVeinModuleRpc::callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &rpcParameters)
{
    emit callFunctionPrivateSignal(callId, peerId, rpcParameters);
}

void cVeinModuleRpc::callFunctionPrivate(const QUuid &callId, const QUuid &peerId, const QVariantMap &rpcParameters)
{
    const auto rpcHandling = [=]() {
        QMutexLocker locker(&(this->m_mutex));

        // check parameters
        QStringList requiredParamList = m_parameter.keys();
        QSet<QString> requiredParamKeys(requiredParamList.begin(), requiredParamList.end());
        const QVariantMap searchParameters = rpcParameters.value(VeinComponent::RemoteProcedureData::s_parameterString).toMap();
        QStringList searchParamList = searchParameters.keys();
        requiredParamKeys.subtract(QSet<QString>(searchParamList.begin(), searchParamList.end()));

        bool callOk = false;
        VeinComponent::RemoteProcedureData::RPCResultCodes resultCode = VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_EINVAL;
        QString errorMsg;
        QVariant returnedResult;
        if(requiredParamKeys.isEmpty()) {
            // call actual function
            QVariantMap rpcParameters = searchParameters;
            rpcParameters.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
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
            returnedResult = rpcParameters;
            resultCode = VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_EINVAL;
            errorMsg = QString("Missing required parameters: [%1]").arg(requiredParamList.join(','));
        }
        m_pendingRpcHash[callId] = peerId;
        if(!callOk || m_functionBlocks) {
            // send answer now
            sendRpcResult(callId, resultCode, errorMsg, returnedResult);
        }
    };

    if(m_threaded) {
        QtConcurrent::run(rpcHandling);
    } else {
        rpcHandling();
    }
}

void cVeinModuleRpc::sendRpcResult(const QUuid &callId, VeinComponent::RemoteProcedureData::RPCResultCodes resultCode, QString errorMsg, QVariant returnedResult)
{
    QHash<QUuid, QUuid>::iterator iter = m_pendingRpcHash.find(callId);
    if(iter != m_pendingRpcHash.end()) {
        QVariantMap returnVal;
        returnVal.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
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

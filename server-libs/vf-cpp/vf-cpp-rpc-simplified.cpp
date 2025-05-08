#include "vf-cpp-rpc-simplified.h"
#include "vf_server_rpc_register.h"

VfCppRpcSimplified::VfCppRpcSimplified(VeinEvent::EventSystem *eventsystem, int entityId, QString rpcName, QMap<QString,QString> parameters) :
    m_eventSystem(eventsystem),
    m_entityId(entityId)
{
    m_rpcSignature = createRpcSignature(rpcName, parameters);
    emit m_eventSystem->sigSendEvent(VfServerRpcRegister::generateEvent(m_entityId, m_rpcSignature));
}

QString VfCppRpcSimplified::createRpcSignature(QString rpcName, QMap<QString, QString> paramDescriptions)
{
    QString signature;
    signature = rpcName;
    signature.append("(");
    const QStringList params = paramDescriptions.keys();
    for(const QString &param : params) {
        signature.append(paramDescriptions[param]);
        signature.append(" ");
        signature.append(param);
        signature.append(",");
    }
    if(signature.at(signature.size()-1) == ",")
        signature.remove(signature.size()-1,1);
    signature.append(")");
    return signature;
}

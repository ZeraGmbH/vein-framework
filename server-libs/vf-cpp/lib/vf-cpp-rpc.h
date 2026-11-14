#ifndef VEINMODULERPC_H
#define VEINMODULERPC_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QUuid>
#include <QSharedPointer>
#include <QMutexLocker>

#include <vcmp_componentdata.h>
#include <vcmp_errordata.h>
#include <ve_eventsystem.h>

#include <QMetaObject>
#include <vcmp_remoteproceduredata.h>
#include <ve_commandevent.h>
#include <vcmp_errordata.h>

namespace VfCpp {

class cVeinModuleRpc: public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer< cVeinModuleRpc > Ptr;
    typedef QMap<QString, QString> Param;

    cVeinModuleRpc(int entityId,
                   VeinEvent::EventSystem *eventsystem,
                   QObject *object,
                   const QString &funcName,
                   const QMap<QString,QString> &parameter,
                   bool threaded = true,
                   bool functionBlocks = true);
    ~cVeinModuleRpc();

    const QString &rpcName() const;

    void callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &rpcParameters);
    void sendRpcResult(const QUuid &callId, VeinComponent::RemoteProcedureData::RPCResultCodes resultCode, QString errorMsg, QVariant returnedResult);

private slots:
    void callFunctionPrivate(const QUuid &callId, const QUuid &peerId, const QVariantMap &rpcParameters);
signals:
    void callFunctionPrivateSignal(const QUuid &callId, const QUuid &peerId, const QVariantMap &rpcParameters);

private:
    QObject *m_object;
    QString m_function;
    QMap<QString,QString> m_parameter;

    QString m_rpcName;

    int m_nEntityId;
    VeinEvent::EventSystem *m_pEventSystem;

    bool m_threaded;
    bool m_functionBlocks;

    QMutex m_mutex;
    QHash<QUuid, QUuid> m_pendingRpcHash;

    friend class VeinEvent::EventSystem;
};

}

#endif // VEINMODULERPC_H

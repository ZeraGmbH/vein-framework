#ifndef VEINMODULERPCMULTITHREADBLOCKED_H
#define VEINMODULERPCMULTITHREADBLOCKED_H

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

namespace VeinEvent
{
class EventSystem;
}

namespace VfCpp {

/**
 * @brief The cVeinModuleRpcMultithreadBlocked class
 */
class cVeinModuleRpcMultithreadBlocked: public QObject
{
    Q_OBJECT
public:
    typedef  QSharedPointer< cVeinModuleRpcMultithreadBlocked > Ptr;
    typedef  QMap<QString, QString> Param;

    cVeinModuleRpcMultithreadBlocked(int entityId,
                   VeinEvent::EventSystem *eventsystem,
                   QObject *p_object,
                   QString p_funcName,
                   QMap<QString,QString> p_parameter,
                   bool p_threaded = true,
                   bool pFunctionBlocks = true);
    ~cVeinModuleRpcMultithreadBlocked();

    QString rpcName() const;

    void callFunction(const QUuid &p_callId, const QUuid &p_peerId, const QVariantMap &t_rpcParameters);
    void sendRpcResult(const QUuid &p_callId, VeinComponent::RemoteProcedureData::RPCResultCodes resultCode, QString errorMsg, QVariant returnedResult);
    static QString createRpcSignature(QString rpcName, QMap<QString,QString> paramDescriptions);

private slots:
    void callFunctionPrivate(const QUuid p_callId, const QUuid p_peerId, const QVariantMap t_rpcParameters);
signals:
    void callFunctionPrivateSignal(const QUuid p_callId, const QUuid p_peerId, const QVariantMap t_rpcParameters);

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

#endif // VEINMODULERPCMULTITHREADBLOCKED_H

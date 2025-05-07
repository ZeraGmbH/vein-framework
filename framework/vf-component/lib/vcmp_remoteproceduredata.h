#ifndef VEINCOMPONENT_REMOTEPROCEDURECALL_H
#define VEINCOMPONENT_REMOTEPROCEDURECALL_H

#include "vfcomponent_export.h"
#include "globalIncludes.h"
#include <ve_eventdata.h>

namespace VeinComponent
{
// RPC QEvent payload
class VFCOMPONENT_EXPORT RemoteProcedureData : public VeinEvent::EventData
{
public:
    enum class Command : qint8 {
        RPCMD_INVALID = -1, /**< default */
        RPCMD_CALL = 0, /**< the call to the remote procedure */
        RPCMD_REGISTER = 1, /**< start introspection for the remote procedure */
        RPCMD_RESULT = 2, /**< result of a remote procedure call */
        RPCMD_PROGRESS = 3, /**< contains progress information and/or partial (streamed) result data */
        RPCMD_CANCELLATION = 4 /**< currently unused */
    };

    enum RPCResultCodes {
        RPC_CANCELED = -64,
        RPC_EINVAL = -EINVAL, //invalid parameters
        RPC_SUCCESS = 0
    };

    /**
     * @brief used to unambiguously store a client scope unique identifier of an invokation
     * @note the CallID is used to track progress or cancellation messages
     */
    static const QLatin1String s_callIdString;
    /**
     * @brief used to unambiguously store the result code in m_invokationData
     * @note the result code can contain anything a QVariant can store (eg. int, string, uuid, empty QVariant, QJSONDocument, etc.)
     */
    static const QLatin1String s_resultCodeString;
    /**
     * @brief used to unambiguously store the error message in m_invokationData
     */
    static const QLatin1String s_errorMessageString;
    /**
     * @brief used to unambiguously store the call parameters
     */
    static const QLatin1String s_parameterString;

    RemoteProcedureData();

    RemoteProcedureData::Command command() const;
    void setCommand(RemoteProcedureData::Command t_command);

    QString procedureName() const;
    void setProcedureName(const QString &t_procedureName);

    QVariantMap invokationData() const;
    void setInvokationData(QVariantMap t_invokationData);

    static constexpr int dataType() { return VCMP_REMOTEPROCEDUREDATA_DATATYPE; }
    int type() const override { return VCMP_REMOTEPROCEDUREDATA_DATATYPE; }
    bool isValid() const override;
    QByteArray serialize() const override;
    void deserialize(const QByteArray &t_data) override;

private:
    Command m_command = Command::RPCMD_INVALID;
    QString m_procedureName;
    /**
     * @brief contains parameters for Command::RPCMD_CALL and also contains the result data for Command::RPCMD_RESULT
     */
    QVariantMap m_invokationData;
};
} // namespace VeinComponent

#endif // VEINCOMPONENT_REMOTEPROCEDURECALL_H

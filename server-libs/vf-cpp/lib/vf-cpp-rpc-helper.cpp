#include "vf-cpp-rpc-helper.h"
#include <QRegularExpression>

QString VfCppRpcHelper::getRpcTypesFromSignature(const QString &rpcSignature)
{
    QRegularExpression re("\\((.*)\\)");
    QRegularExpressionMatch match = re.match(rpcSignature);
    if (match.hasMatch()) {
        QString params = match.captured(1).trimmed();
        QStringList parts = params.split(',', Qt::SkipEmptyParts);
        for (QString &p : parts) {
            p = p.trimmed();
            p.remove(QRegularExpression(R"(\s+\w+$)"));
        }
        return parts.join(',');
    }
    return QString();
}

QStringList VfCppRpcHelper::getRpcTypesListFromSignature(const QString &rpcSignature)
{
    QString typesCommaSeparated = getRpcTypesFromSignature(rpcSignature);
    return typesCommaSeparated.split(",", Qt::SkipEmptyParts);
}

QStringList VfCppRpcHelper::getRpcParamNamesList(const QString &RPC)
{
    // RPC_name(type p_paramName1, type p_paramName2)
    QStringList parameterNames;
    QRegularExpression re("\\((.*)\\)");
    QRegularExpressionMatch match = re.match(RPC);

    if (match.hasMatch()) {
        QString params = match.captured(1).trimmed();
        const QStringList paramsList = params.split(",", Qt::SkipEmptyParts);

        for (const QString &parameter : paramsList) {
            QStringList paramTypeValue = parameter.split(" ");
            if (paramTypeValue.size() == 2)
                parameterNames.append(paramTypeValue[1]);
        }
    }
    return parameterNames;
}

QString VfCppRpcHelper::getRpcName(const QString &RPC)
{
    QString RPCName = RPC.section('(', 0, 0);
    return RPCName;
}

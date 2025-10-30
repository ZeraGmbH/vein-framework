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

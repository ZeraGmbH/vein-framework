#include "testdumpreporter.h"
#include <QString>
#include <QJsonDocument>

bool TestDumpReporter::compareAndLogOnDiff(QByteArray expected, QByteArray dumped)
{
    if(expected != dumped) {
        qWarning("Expected:");
        qInfo("%s", qPrintable(expected));
        qWarning("Dumped:");
        qInfo("%s", qPrintable(dumped));
        return false;
    }
    return true;
}

QByteArray TestDumpReporter::dump(QJsonObject json)
{
    QJsonDocument doc(json);
    return doc.toJson(QJsonDocument::Indented);
}

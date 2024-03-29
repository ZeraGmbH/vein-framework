#include "testdumpreporter.h"
#include <QString>
#include <QJsonDocument>

bool TestDumpReporter::reportOnFail(QByteArray jsonExpected, QByteArray jsonDumped)
{
    if(jsonExpected != jsonDumped) {
        qWarning("Expected:");
        qInfo("%s", qPrintable(jsonExpected));
        qWarning("Dumped:");
        qInfo("%s", qPrintable(jsonDumped));
        return false;
    }
    return true;
}

QByteArray TestDumpReporter::dump(QJsonObject json)
{
    QJsonDocument doc(json);
    return doc.toJson(QJsonDocument::Indented);
}

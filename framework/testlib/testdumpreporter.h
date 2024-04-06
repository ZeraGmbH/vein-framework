#ifndef TESTDUMPREPORTER_H
#define TESTDUMPREPORTER_H

#include <QByteArray>
#include <QJsonObject>

class TestDumpReporter
{
public:
    static bool compareAndLogOnDiff(QByteArray expected, QByteArray dumped);
    static QByteArray dump(QJsonObject json);
};

#endif // TESTDUMPREPORTER_H

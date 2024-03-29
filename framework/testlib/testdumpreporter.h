#ifndef TESTDUMPREPORTER_H
#define TESTDUMPREPORTER_H

#include <QByteArray>
#include <QJsonObject>

class TestDumpReporter
{
public:
    static bool reportOnFail(QByteArray jsonExpected, QByteArray jsonDumped);
    static QByteArray dump(QJsonObject json);
};

#endif // TESTDUMPREPORTER_H

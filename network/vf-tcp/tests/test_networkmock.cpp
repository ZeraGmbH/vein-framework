#include "test_networkmock.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_networkmock)


void test_networkmock::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}


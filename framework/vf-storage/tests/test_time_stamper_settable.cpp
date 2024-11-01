#include "test_time_stamper_settable.h"
#include "vs_timestampersettable.h"
#include <timemachinefortest.h>
#include <timerfactoryqtfortest.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_time_stamper_settable)

using namespace VeinStorage;

void test_time_stamper_settable::initTestCase()
{
    TimerFactoryQtForTest::enableTest();
}

void test_time_stamper_settable::init()
{
    TimeMachineForTest::reset();
}

void test_time_stamper_settable::initTimeIsNow()
{
    TimeStamperSettablePtr stamper = TimeStamperSettable::create();
    TimeMachineForTest::getInstance()->processTimers(42);

    QDateTime timestamp = stamper->getTimestamp();
    QDateTime timestampExpected = msAfterEpoch(0);
    QCOMPARE(timestamp, timestampExpected);
}

void test_time_stamper_settable::delayAndSetStamp()
{
    TimeStamperSettablePtr stamper = TimeStamperSettable::create();
    TimeMachineForTest::getInstance()->processTimers(42);

    QDateTime timestamp0 = stamper->getTimestamp();
    QDateTime timestampExpected0 = msAfterEpoch(0);
    QCOMPARE(timestamp0, timestampExpected0);

    stamper->setTimestampToNow();
    QDateTime timestamp42 = stamper->getTimestamp();
    QDateTime timestampExpected42 = msAfterEpoch(42);
    QCOMPARE(timestamp42, timestampExpected42);
}

void test_time_stamper_settable::signalOnSet()
{
    TimeStamperSettablePtr stamper = TimeStamperSettable::create();
    QSignalSpy spy(stamper.get(), &TimeStamperSettable::sigTimestampChanged);
    TimeMachineForTest::getInstance()->processTimers(42);

    stamper->setTimestampToNow();
    QCOMPARE(spy.count(), 1);
}

void test_time_stamper_settable::noSignalOnSameTimeSet()
{
    TimeStamperSettablePtr stamper = TimeStamperSettable::create();
    QSignalSpy spy(stamper.get(), &TimeStamperSettable::sigTimestampChanged);
    TimeMachineForTest::getInstance()->processTimers(42);

    stamper->setTimestampToNow();
    stamper->setTimestampToNow();
    QCOMPARE(spy.count(), 1);
}

QDateTime test_time_stamper_settable::msAfterEpoch(qint64 msecs)
{
    QDateTime dateTime;
    dateTime.setMSecsSinceEpoch(msecs);
    return dateTime;
}

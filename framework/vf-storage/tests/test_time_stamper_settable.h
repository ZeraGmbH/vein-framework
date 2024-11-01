#ifndef TEST_TIME_STAMPER_SETTABLE_H
#define TEST_TIME_STAMPER_SETTABLE_H

#include <QObject>
#include <QDateTime>

class test_time_stamper_settable : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void initTimeIsNow();
    void delayAndSetStamp();
    void signalOnSet();
    void noSignalOnSameTimeSet();
private:
    QDateTime msAfterEpoch(qint64 msecs);
};

#endif // TEST_TIME_STAMPER_SETTABLE_H

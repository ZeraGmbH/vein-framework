#ifndef TEST_STORAGE_NOTIFICATION_H
#define TEST_STORAGE_NOTIFICATION_H

#include <testveinserver.h>
#include <QObject>
#include <QVariant>

class test_storage_notification : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void getNullNotifierFromEmpty();
    void getNotifierForExisting();
    void getNotifierForNonExisting();
    void getNotifierForTwoExisting();
    void receiveOneChangeSignalPerChangeByVein();
    void receiveNoChangeSignalOnSameValueByVein();
private:
    void sendVeinSetAndProcess(TestVeinServer *server, QVariant oldValue, QVariant newValue);
};

#endif // TEST_STORAGE_NOTIFICATION_H

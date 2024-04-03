#ifndef TEST_STORAGE_NOTIFICATION_H
#define TEST_STORAGE_NOTIFICATION_H

#include <QObject>

class test_storage_notification : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void getNullNotifierFromEmpty();
    void getNotifierForExisting();
    void getNotifierForNonExisting();
    void getNotifierForExistingTwice();
    void getNotifierForTwoExisting();
};

#endif // TEST_STORAGE_NOTIFICATION_H

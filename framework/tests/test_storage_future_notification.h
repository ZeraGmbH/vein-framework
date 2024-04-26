#ifndef TEST_STORAGE_FUTURE_NOTIFICATION_H
#define TEST_STORAGE_FUTURE_NOTIFICATION_H

#include <QObject>

class test_storage_future_notification : public QObject
{
    Q_OBJECT
private slots:
    void getComponentOnEmptyHash();
    void getComponentAddedLater();
    void getComponentsAddedLaterIdentity();
    void getComponentAddedLaterIdentity();
    void getFutureComponentAlreadyStored();
    void checkChangeSignals();
};

#endif // TEST_STORAGE_FUTURE_NOTIFICATION_H

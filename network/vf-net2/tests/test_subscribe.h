#ifndef TEST_SUBSCRIBE_H
#define TEST_SUBSCRIBE_H

#include <QObject>

class test_subscribe : public QObject
{
    Q_OBJECT
private slots:
    void intropectSendToSubscriberOnly();
};

#endif // TEST_SUBSCRIBE_H

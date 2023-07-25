#ifndef TEST_VFSIMPLEENTITYSUBSCRIBER_H
#define TEST_VFSIMPLEENTITYSUBSCRIBER_H

#include <QObject>

class test_vfsimpleentitysubscriber : public QObject
{
    Q_OBJECT
private slots:
    void intropectSystemEntitySignalReceived();
    void trySubscribeOnNonExistantEntity();
    void introspectComponentNames();
private:
    void feedEventLoop();
};

#endif // TEST_VFSIMPLEENTITYSUBSCRIBER_H

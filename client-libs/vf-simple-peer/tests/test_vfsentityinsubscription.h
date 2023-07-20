#ifndef TEST_VFSENTITYINSUBSCRIPTION_H
#define TEST_VFSENTITYINSUBSCRIPTION_H

#include <QObject>

class test_vfsentityinsubscription : public QObject
{
    Q_OBJECT
private slots:
    void intropectSystemEntitySignalReceived();
    void trySubscribeOnNonExistantEntity();
private:
    void feedEventLoop();
};

#endif // TEST_VFSENTITYINSUBSCRIPTION_H

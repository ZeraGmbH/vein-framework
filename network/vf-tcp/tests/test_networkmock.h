#ifndef TEST_NETWORKMOCK_H
#define TEST_NETWORKMOCK_H

#include <QObject>

class test_networkmock : public QObject
{
    Q_OBJECT
private slots:
private:
    void feedEventLoop();
};

#endif // TEST_NETWORKMOCK_H

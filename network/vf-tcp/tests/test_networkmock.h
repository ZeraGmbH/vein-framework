#ifndef TEST_NETWORKMOCK_H
#define TEST_NETWORKMOCK_H

#include <QObject>

class test_networkmock : public QObject
{
    Q_OBJECT
private slots:
    // We need this to learn howto set up a working syssytem
    // before starting on network mock. In case we are done
    // and this test turns fragile, it can be removed
    void receiveIntrospectionOverRealNetwork();
    void receiveIntrospectionOverMockNetwork();
private:
    void feedEventLoop();
};

#endif // TEST_NETWORKMOCK_H

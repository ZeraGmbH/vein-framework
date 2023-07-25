#ifndef TEST_VEINNETWORKSTACKS_H
#define TEST_VEINNETWORKSTACKS_H

#include <QObject>

class test_veinnetworkstacks : public QObject
{
    Q_OBJECT
private slots:
    // We need this to learn howto set up a working syssytem
    // before starting on network mock. In case we are done
    // and this test turns fragile, it can be removed
    void receiveIntrospection();
private:
    void feedEventLoop();
};

#endif // TEST_VEINNETWORKSTACKS_H

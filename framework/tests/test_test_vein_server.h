#ifndef TEST_TEST_VEIN_SERVER_H
#define TEST_TEST_VEIN_SERVER_H

#include "testveinserver.h"
#include <QObject>
#include <memory>

class test_test_vein_server : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void unfinishedSession();
    void completeSession();
private:
    std::unique_ptr<TestVeinServer> m_server;
};

#endif // TEST_TEST_VEIN_SERVER_H

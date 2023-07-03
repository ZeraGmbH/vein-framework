#ifndef TEST_VF_CPP_CLIENT_H
#define TEST_VF_CPP_CLIENT_H

#include <QObject>

class test_vf_cpp_client : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void introspectionTest();
private:
    void feedEventLoop();
};

#endif // TEST_VF_CPP_CLIENT_H

#ifndef TEST_CORE_STACK_CLIENT_H
#define TEST_CORE_STACK_CLIENT_H

#include <QObject>

class test_core_stack_client : public QObject
{
    Q_OBJECT
private slots:
    void subsriptionWithThrowAwayObjects();
};

#endif // TEST_CORE_STACK_CLIENT_H

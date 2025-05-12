#ifndef TEST_RPC_SIMPLIFIED_H
#define TEST_RPC_SIMPLIFIED_H

#include <QObject>

class test_rpc_simplified : public QObject
{
    Q_OBJECT
private slots:
    void callRpc();
};

#endif // TEST_RPC_SIMPLIFIED_H

#ifndef TEST_RPC_SIGNATURE_H
#define TEST_RPC_SIGNATURE_H

#include <QObject>

class test_rpc_signature : public QObject
{
    Q_OBJECT
private slots:
    void singleParam();
    void twoParams();
};

#endif // TEST_RPC_SIGNATURE_H

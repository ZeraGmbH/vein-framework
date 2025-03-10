#ifndef TEST_CLIENT_RPC_INVOKER_H
#define TEST_CLIENT_RPC_INVOKER_H

#include <QObject>

class test_client_rpc_invoker : public QObject
{
    Q_OBJECT
private slots:
    void errorSignalFromUnsubscribedEntityInvalidRPCNoNet();
    void unsubscribedEntityValidRPCNoNet();
    void unsubscribedEntityValidRPCNet();
    void subscribedEntityValidRPCNet();
    void subscribedEntityNonExistentRPCNet();
};

#endif // TEST_CLIENT_RPC_INVOKER_H

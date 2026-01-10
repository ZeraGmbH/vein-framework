#ifndef TEST_SERVER_RPC_INVOKER_H
#define TEST_SERVER_RPC_INVOKER_H

#include <QObject>

class test_server_rpc_invoker : public QObject
{
    Q_OBJECT
private slots:
    void invalidRPCNoNet();
    void invalidRPCNoNetNoCrossTalk();
    void validRPCNoNet();

    //Do we need server net ? no client needed !
    void invalidRPCNet();
    void validRPCNet();
    void validRPCTwice();
    void noCrossTalk();
};

#endif // TEST_SERVER_RPC_INVOKER_H

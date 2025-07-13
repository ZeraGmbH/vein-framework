#ifndef TEST_NETWORK_FRAGMENTED_TRANSFER_H
#define TEST_NETWORK_FRAGMENTED_TRANSFER_H

#include <QObject>

class test_network_fragmented_transfer : public QObject
{
    Q_OBJECT
private slots:
    void init();

    void testTestPeerWorkerNotFragmented();
    void testTestPeerWorkerHugeFragment();

    void sendServerOneMsgFragmented();
    void sendServerTwoMsgFragmentedOneTransaction();
    void sendServerTwoMsgFragmentedTwoTransactions();
    void sendClientOneMsgFragmented();
    void sendClientTwoMsgFragmentedOneTransaction();
    void sendClientTwoMsgFragmentedTwoTransactions();
};

#endif // TEST_NETWORK_FRAGMENTED_TRANSFER_H

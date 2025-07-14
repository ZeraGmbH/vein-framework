#ifndef TEST_NETWORK_FRAGMENTED_TRANSFER_H
#define TEST_NETWORK_FRAGMENTED_TRANSFER_H

#include <QObject>
#include <QList>
#include <QByteArray>

typedef QList<QByteArray> SendList;

class test_network_fragmented_transfer : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase_data();
    void init();

    void testTestPeerWorkerNotFragmented();
    void testTestPeerWorkerHugeFragment();

    void sendServerOneMsgFragmented();
    void sendServerTwoMsgFragmentedOneTransaction();
    void sendServerTwoMsgFragmentedTwoTransactions();
    void sendClientOneMsgFragmented();
    void sendClientTwoMsgFragmentedOneTransaction();
    void sendClientTwoMsgFragmentedTwoTransactions();
private:
    int calcTransactionCount(const SendList &transferredStrings);

    qint64 m_fragmentedSize = 0;
};

#endif // TEST_NETWORK_FRAGMENTED_TRANSFER_H

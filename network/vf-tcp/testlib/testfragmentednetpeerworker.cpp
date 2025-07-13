#include "testfragmentednetpeerworker.h"
#include <timersingleshotqt.h>
#include <QDataStream>

static constexpr int fragmentTimeSendMs = 10;

namespace VeinTcp
{

int TestFragmentedNetPeerWorker::m_readyReadCount = 0;

// client side
TestFragmentedNetPeerWorker::TestFragmentedNetPeerWorker(TcpPeer *peer, qint64 writeSize) :
    TcpPeerWorker(peer),
    m_writeSize(writeSize),
    m_pendingSendData(std::make_unique<QByteArray>()),
    m_pendingSendTimer(std::make_unique<TimerSingleShotQt>(fragmentTimeSendMs))
{
    doConnections();
}

// server side
TestFragmentedNetPeerWorker::TestFragmentedNetPeerWorker(TcpPeer *peer, qintptr socketDescriptor, qint64 writeSize) :
    TcpPeerWorker(peer, socketDescriptor),
    m_writeSize(writeSize),
    m_pendingSendData(std::make_unique<QByteArray>()),
    m_pendingSendTimer(std::make_unique<TimerSingleShotQt>(fragmentTimeSendMs))
{
    doConnections();
}

void TestFragmentedNetPeerWorker::doConnections()
{
    connect(m_pendingSendTimer.get(), &TimerTemplateQt::sigExpired,
            this, &TestFragmentedNetPeerWorker::onPendingTimer);
    connect(m_tcpSock, &QTcpSocket::readyRead,
            this, &TestFragmentedNetPeerWorker::handleReadyRead);
}

void VeinTcp::TestFragmentedNetPeerWorker::sendFraction() const
{
    QByteArray bytesSendNow = m_pendingSendData->left(m_writeSize);
    writeRaw(bytesSendNow);

    int byteCountPending = m_pendingSendData->size() - bytesSendNow.size();
    QByteArray bytesPending = m_pendingSendData->right(byteCountPending);
    *m_pendingSendData = bytesPending;

    if (byteCountPending > 0)
        m_pendingSendTimer->start();
}

void TestFragmentedNetPeerWorker::sendArray(const QByteArray &byteArray) const
{
    if(!isConnected()) {
        qWarning("[vein-tcp] Trying to send data to disconnected host.");
        return;
    }
    QByteArray rawData;
    QDataStream out(&rawData, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << byteArray;

    if (m_writeSize == 0)
        writeRaw(rawData);
    else {
        m_pendingSendData->append(rawData);
        if(!m_pendingSendTimer->isRunning())
            sendFraction();
    }
}

int TestFragmentedNetPeerWorker::getReadyReadCount()
{
    return m_readyReadCount;
}

void TestFragmentedNetPeerWorker::resetReadyReadCount()
{
    m_readyReadCount = 0;
}

void TestFragmentedNetPeerWorker::onPendingTimer()
{
    sendFraction();
}

void TestFragmentedNetPeerWorker::handleReadyRead()
{
    m_readyReadCount++;
}

}

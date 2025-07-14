#include "testfragmentednetpeerworker.h"
#include <timersingleshotqt.h>
#include <QDataStream>

namespace VeinTcp
{

int TestFragmentedNetPeerWorker::m_readyReadCount = 0;

// client side
TestFragmentedNetPeerWorker::TestFragmentedNetPeerWorker(TcpPeer *peer, qint64 writeSize) :
    TcpPeerWorker(peer),
    m_writeSize(writeSize),
    m_pendingDataOnSocket(std::make_unique<qint64>(0)),
    m_pendingSendData(std::make_unique<QByteArray>())
{
    doConnections();
}

// server side
TestFragmentedNetPeerWorker::TestFragmentedNetPeerWorker(TcpPeer *peer, qintptr socketDescriptor, qint64 writeSize) :
    TcpPeerWorker(peer, socketDescriptor),
    m_writeSize(writeSize),
    m_pendingDataOnSocket(std::make_unique<qint64>(0)),
    m_pendingSendData(std::make_unique<QByteArray>())
{
    doConnections();
}

void TestFragmentedNetPeerWorker::doConnections()
{
    connect(m_tcpSock, &QTcpSocket::readyRead,
            this, &TestFragmentedNetPeerWorker::handleReadyRead);
    connect(m_tcpSock, &QTcpSocket::bytesWritten,
            this, &TestFragmentedNetPeerWorker::onBytesWritten, Qt::QueuedConnection);
}

void VeinTcp::TestFragmentedNetPeerWorker::sendFraction() const
{
    qint64 maxBytesSendableNowCount = m_writeSize - *m_pendingDataOnSocket;
    if (maxBytesSendableNowCount > 0) {
        qint64 bytesSendNowCount = std::min(maxBytesSendableNowCount, (qint64)m_pendingSendData->size());
        if (bytesSendNowCount > 0) {
            QByteArray bytesSendNow = m_pendingSendData->left(bytesSendNowCount);
            writeRaw(bytesSendNow);
            *m_pendingDataOnSocket += bytesSendNowCount;

            int byteCountPending = m_pendingSendData->size() - bytesSendNowCount;
            QByteArray bytesPending = m_pendingSendData->right(byteCountPending);
            *m_pendingSendData = bytesPending;
        }
    }
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

    if (m_writeSize == 0) {
        writeRaw(rawData);
        *m_pendingDataOnSocket += rawData.count();
    }
    else {
        m_pendingSendData->append(rawData);
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

void TestFragmentedNetPeerWorker::onBytesWritten(qint64 bytes)
{
    *m_pendingDataOnSocket -= bytes;
    if (*m_pendingDataOnSocket == 0 && m_pendingSendData->size())
        sendFraction();
}

void TestFragmentedNetPeerWorker::handleReadyRead()
{
    m_readyReadCount++;
}

}

#ifndef PROTOPEERPRIVATE_H
#define PROTOPEERPRIVATE_H

#include "xiqnetwrapper.h"

#include <QUuid>

class XiQNetPeer;

class QTcpSocket;

class XiQNetPeerPrivate
{
private:
    explicit XiQNetPeerPrivate(XiQNetPeer *t_publicPeer);

    QByteArray readArray();
    void sendArray(const QByteArray &t_byteArray) const;
    QUuid m_peerId;

    qint32 m_socketBufferExpectedSize = 0;
    QTcpSocket *m_tcpSock = nullptr;
    XiQNetPeer *q_ptr = nullptr;

    friend class XiQNetPeer;
};

#endif // PROTOPEERPRIVATE_H

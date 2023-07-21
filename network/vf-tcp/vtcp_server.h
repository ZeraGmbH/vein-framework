#ifndef VEIN_TCP_SERVER_H
#define VEIN_TCP_SERVER_H

#include "vftcp_export.h"
#include <QObject>
#include <QList>

namespace VeinTcp
{
class TcpServerPrivate;
class TcpPeer;

/**
 * @brief Custom QTcpServer based implementation of a TCP server
 */
class VFTCP_EXPORT TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *t_parent = nullptr);
    virtual ~TcpServer();
    QList<TcpPeer*> getClientList() const;
    bool isListening() const;
signals:
    void sigClientConnected(TcpPeer *t_peer);
public slots:
    void broadcastMessage(QByteArray t_message) const;
    /**
     * @brief Convenient function for QTcpServer::listen()
     * @note Prints out the server port to the debug message handler
     * @param t_port: Port number
     * @param t_systemdSocket: true: use systemd socket
     */
    bool startServer(quint16 t_port, bool t_systemdSocket=true);
protected slots:
    /**
     * @brief For reference housekeeping
     */
    void clientDisconnectedSRV(TcpPeer *t_peer);
private:
    void incomingConnection(qintptr t_socketDescriptor);
    friend class TcpServerPrivate;
    TcpServerPrivate *d_ptr = nullptr;
};
}
#endif // VEIN_TCP_SERVER_H

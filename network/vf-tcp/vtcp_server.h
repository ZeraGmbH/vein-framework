#ifndef VEIN_TCP_SERVER_H
#define VEIN_TCP_SERVER_H

#include "vftcp_export.h"

#include <QTcpServer>

#include <QList>

namespace VeinTcp
{
  class TcpServerPrivate;
  class TcpPeer;

  /**
 * @brief Custom QTcpServer based implementation of a TCP server
 */
  class VFTCP_EXPORT TcpServer : public QTcpServer
  {
    Q_OBJECT
  public:
    explicit TcpServer(QObject *t_parent = nullptr);
    virtual ~TcpServer();

    QList<TcpPeer*> getClientList() const;

  signals:
    /**
   * @brief A new client connected
   * @param t_peer
   */
    void sigClientConnected(TcpPeer *t_peer);

  public slots:
    /**
   * @brief Sends the message to all peers of this server
   * @param t_message
   */
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

  protected:
    /**
   * @brief Override of QTcpServer function
   * @param t_socketDescriptor
   */
    void incomingConnection(qintptr t_socketDescriptor) override;

  private:
    /**
   * @brief PIMPL pointer
   */
    TcpServerPrivate *d_ptr = nullptr;
  };
}
#endif // VEIN_TCP_SERVER_H

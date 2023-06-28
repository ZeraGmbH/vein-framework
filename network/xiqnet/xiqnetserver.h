#ifndef H2012_xiqnet_SERVER_PUB_H
#define H2012_xiqnet_SERVER_PUB_H

#include "xiqnet_global.h"
#include "xiqnet_export.h"

#include <QTcpServer>

#include <QList>

class XiQNetServerPrivate;
class XiQNetPeer;
class XiQNetWrapper;

class XIQNET_EXPORT XiQNetServer : public QTcpServer
{
  Q_OBJECT
public:
  explicit XiQNetServer(QObject *t_parent = nullptr);
  virtual ~XiQNetServer();

  QList<XiQNetPeer*> getClientList() const;

  /**
   * @brief Incoming connections use the default XiQNetWrapper for messaging
   * @param t_wrapper
   */
  void setDefaultWrapper(XiQNetWrapper *t_wrapper);

signals:
  /**
   * @brief A new client connected
   * @param t_peer
   */
  void sigClientConnected(XiQNetPeer *t_peer);

public slots:
  /**
   * @brief Sends the message to all peers of this server
   * @param t_message
   */
  void broadcastMessage(const google::protobuf::Message &t_message) const;

  /**
   * @brief Convenient function for QTcpServer::listen()
   * @note Prints out the server port to the debug message handler
   * @param t_port: Port number
   * @param t_systemdSocket: true: use systemd socket
   */
  void startServer(quint16 t_port, bool t_systemdSocket=true);

protected slots:
  /**
   * @brief For reference housekeeping
   */
  void clientDisconnectedSRV();

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
  XiQNetServerPrivate *d_ptr = nullptr;
};

#endif // H2012_xiqnet_SERVER_H

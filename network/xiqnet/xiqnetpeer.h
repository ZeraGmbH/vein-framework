#ifndef PROTOPEER_H
#define PROTOPEER_H

#include "xiqnet_global.h"
#include "xiqnet_export.h"

#include <QObject>
#include <QString>
#include <QAbstractSocket>

class XiQNetPeerPrivate;
class XiQNetWrapper;
class QTcpSocket;

/**
 * @brief Custom QTcpSocket based implementation of a TCP network peer
 */
class XIQNET_EXPORT XiQNetPeer : public QObject
{
  Q_OBJECT
public:
  explicit XiQNetPeer(QObject *t_parent = nullptr);
  explicit XiQNetPeer(qintptr t_socketDescriptor, QObject *t_parent = nullptr);

  ~XiQNetPeer();

  QString getIpAddress() const;
  quint16 getPort() const;
  bool isConnected() const;


  int getPeerId() const;
  void setPeerId(int t_peerId);

  /**
   * @brief Allows access to the QTcpSocket in case of non protobuf transmission
   * @return QTcpSocket used by this instance
   * @todo refactor, as this violates the OOP principle "separation of concerns"
   */
  QTcpSocket *getTcpSocket() const;


  /**
   * @brief Returns a reference to the used XiQNetWrapper
   * @return
   */
  XiQNetWrapper *getWrapper() const;
  /**
   * @brief Sets the XiQNetWrapper for this peer
   * @param value
   */
  void setWrapper(XiQNetWrapper *value);

  QString getErrorString() const;

signals:
  /**
   * @brief Emitted if successful connected
   */
  void sigConnectionEstablished();
  /**
   * @brief Emitted when the connection is interrupted
   */
  void sigConnectionClosed();
  /**
   * @brief Emitted when incoming messages arrive
   * @param t_Message
   * @bug Do not use the t_Message parameter with Qt::QueuedConnection as it may be deleted before the slot is called
   */
  void sigMessageReceived(std::shared_ptr<google::protobuf::Message> t_Message);
  /**
   * @brief Emitted on socket failure
   * @param t_socketError
   */
  void sigSocketError(QAbstractSocket::SocketError t_socketError);

public slots:
  /**
   * @brief Transmits the message
   * @param t_message
   */
  void sendMessage(const google::protobuf::Message &t_message) const;
  /**
   * @brief Starts the connection in case of a dedicated host connection
   *
   * Called after Signal/Slot setup
   * @param t_ipAddress
   * @param t_port
   */
  void startConnection(QString t_ipAddress, quint16 t_port);
  /**
   * @brief Closes the connection
   */
  void stopConnection();

protected slots:
  /**
   * @brief Translates incoming messages to google::protobuf::Message via the XiQNetWrapper
   */
  void onReadyRead();

private:
  /**
   * @brief PIMPL pointer
   */
  XiQNetPeerPrivate *d_ptr = nullptr;
};

#endif // PROTOPEER_H

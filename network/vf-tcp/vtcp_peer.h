#ifndef VEIN_TCP_PEER_H
#define VEIN_TCP_PEER_H

#include "vftcp_export.h"

#include <QObject>
#include <QString>
#include <QAbstractSocket>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace VeinTcp
{
  class TcpPeerPrivate;


  /**
 * @brief Custom QTcpSocket based implementation of a TCP network peer
 */
  class VFTCP_EXPORT TcpPeer : public QObject
  {
    Q_OBJECT
  public:
    explicit TcpPeer(QObject *t_parent = nullptr);
    explicit TcpPeer(qintptr t_socketDescriptor, QObject *t_parent = nullptr);

    ~TcpPeer();

    QString getIpAddress() const;
    quint16 getPort() const;
    bool isConnected() const;


    QUuid getPeerId() const;
    void setPeerId(QUuid t_peerId);

    /**
     * @brief Forwards QTcpSocket->errorString();
     * @return Error string
     */
    QString getErrorString() const;

  signals:
    /**
     * @brief Emitted if successful connected
     * @param t_sender the connected peer
     */
    void sigConnectionEstablished(TcpPeer *t_sender);
    /**
     * @brief Emitted when the connection is interrupted
     * @param t_sender dthe disconnected peer
     */
    void sigConnectionClosed(TcpPeer *t_sender);
    /**
     * @brief Emitted when incoming messages arrive
     * @param t_sender peer who sent the message
     * @param t_message message data
     */
    void sigMessageReceived(TcpPeer *t_sender, QByteArray t_message);
    /**
     * @brief Emitted on socket failure
     * @param t_sender peer where the fault occured
     * @param t_socketError
     */
    void sigSocketError(TcpPeer *t_sender, QAbstractSocket::SocketError t_socketError);

  public slots:
    /**
   * @brief Transmits the message
   * @param t_message
   */
    void sendMessage(QByteArray t_message) const;
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
    void onReadyRead();

  private:
    /**
   * @brief PIMPL pointer
   */
    TcpPeerPrivate *d_ptr = nullptr;
  };
}

#endif // VEIN_TCP_PEER_H

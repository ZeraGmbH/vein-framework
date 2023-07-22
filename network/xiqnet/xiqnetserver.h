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
    void setDefaultWrapper(XiQNetWrapper *t_wrapper);
signals:
    void sigClientConnected(XiQNetPeer *t_peer);
public slots:
    void startServer(quint16 t_port, bool t_systemdSocket=true);

private slots:
    void clientDisconnectedSRV();
private:
    void incomingConnection(qintptr t_socketDescriptor) override;
    XiQNetServerPrivate *d_ptr = nullptr;
};

#endif // H2012_xiqnet_SERVER_H

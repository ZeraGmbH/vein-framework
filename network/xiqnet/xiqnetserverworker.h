#ifndef XIQNETSERVERWORKER_H
#define XIQNETSERVERWORKER_H

#include "vtcp_serverworkerinterface.h"
#include "xiqnetserver.h"
#include "xiqnetpeer.h"
#include <QList>
#include <QTcpServer>

namespace VeinTcp
{
class XiQNetServerWorker : public QTcpServer, public TcpServerWorkerInterface
{
    Q_OBJECT
private:
    // Just our friend XiqnetWorkerFactoryMethods can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    /*struct secret { explicit secret() = default; };
    friend class XiqnetWorkerFactoryMethods;*/
public:
    XiQNetServerWorker(XiQNetServer *server/*, secret*/);
    ~XiQNetServerWorker();
    bool startServer(quint16 port, bool systemdSocket) override;
    bool isListenActive() override;
private slots:
    void clientDisconnectedSRV(XiQNetPeer *peer);
private:
    void incomingConnection(qintptr socketDescriptor) override;
    QList<XiQNetPeer*> m_clients;
    XiQNetServer *m_server = nullptr;
};
}

#endif // XIQNETSERVERWORKER_H

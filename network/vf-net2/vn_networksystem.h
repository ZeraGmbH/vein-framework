#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "globalIncludes.h"
#include <ve_eventsystem.h>

namespace VeinNet
{
class NetworkSystemPrivate;
/**
   * @brief Turns all sort of events into ProtocolEvents and sends them depending on the OperationMode
   *
   * Also translates remote ProtocolEvents into regular events and posts them to the EventHandler
   */
class VFNET2_EXPORT NetworkSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit NetworkSystem(QObject *parent = nullptr);
    virtual ~NetworkSystem();

    enum OperationMode {
        VNOM_DEBUG =0, /**< do nothing and only print the debug message */
        VNOM_PASS_THROUGH, /**< pass all events to the other site */
        VNOM_SUBSCRIPTION /**< [default] only pass events when the other site subscribed to it */
    };
    OperationMode operationMode() const;
    void setOperationMode(const OperationMode &operationMode);

    int getSubscriberCount(int entityId) const;
public:
    void processEvent(QEvent *event) override;
private:
    NetworkSystemPrivate *d_ptr = nullptr;
};
}

#endif // CONNECTIONMANAGER_H

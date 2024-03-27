#ifndef VE_COMMANDEVENT_H
#define VE_COMMANDEVENT_H

#include "vfevent_export.h"
#include <QEvent>
#include <QUuid>
#include <memory>

namespace VeinEvent
{
class EventData;

// The CommandEvent class is a container for component and entity changes
class VFEVENT_EXPORT CommandEvent : public QEvent
{
public:
    enum class EventSubtype : int {
        NOTIFICATION = 0, /**< The event meets the authoritative requirements (if any) */
        TRANSACTION = 1 /**< The event requires to be inspected about its authoritative status */
    };

    explicit CommandEvent(EventSubtype subtype, EventData *data);
    ~CommandEvent();
    void mutateInto(std::unique_ptr<VeinEvent::CommandEvent> otherCmdEvent);

    static int getQEventType();

    QUuid peerId() const;
    void setPeerId(QUuid pPeerId);

    EventSubtype eventSubtype() const;
    void setEventSubtype(EventSubtype newType);

    EventData *eventData() const;

private:
    static const int m_registeredQEventType;

    /**
     * @brief Depending on the context of the event this may be the sender or receiver of the event
     * @note Validator may use this data to check for permissions, an introspection system may use this data for sender/receiver addressing
     */
    QUuid m_peerId;
    EventSubtype m_subtype;
    EventData *m_eventData;
    std::unique_ptr<VeinEvent::CommandEvent> m_mutationCmdEvent;
    VeinEvent::CommandEvent* m_activeEvent;
};
}
#endif // VE_COMMANDEVENT_H

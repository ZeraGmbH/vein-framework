#ifndef VEINEVENT_VE_EVENTDATA_H
#define VEINEVENT_VE_EVENTDATA_H

#include "vfevent_export.h"
#include "globalIncludes.h"
#include <QByteArray>

namespace VeinEvent
{
/**
   * @brief The data representing component and entity changes, designed like the Command pattern from the GoF book
   */
class VFEVENT_EXPORT EventData
{
public:
    EventData();
    virtual ~EventData() {}

    enum class EventOrigin : qint8 {
        EO_LOCAL = 0, /**< the event is from the local system and generally trustworthy */
        EO_FOREIGN = 1, /**< the event is from a remote client or generally untrustworthy */
        EO_USER_DEFINED = 64 /**< the event is from a user defined origin if >= EO_USER_DEFINED */
    };

    EventOrigin eventOrigin() const;
    void setEventOrigin(EventOrigin t_eventOrigin);

    enum class EventTarget : qint8 {
        ET_IRRELEVANT = 0, /**< the target is irrelevant for incoming remote events */
        ET_LOCAL = 1, /**< flags that the event should not be transmitted over the network border */
        ET_ALL = 2, /**< the event can be transmitted freely */
        ET_USER_DEFINED = 64 /**< the event is for a user defined target if >= ET_USER_DEFINED */
    };

    EventTarget eventTarget() const;
    void setEventTarget(EventTarget t_eventTarget);

    virtual int type() const =0;

    virtual bool isValid() const;

    /**
     * @brief Used to encode the class in binary information form
     * @note m_eventTarget and m_eventOrigin are not encoded as they are local only informations
     * @return
     */
    virtual QByteArray serialize() const = 0;

    /**
     * @brief Used to decode the class from binary information form
     * @note m_eventTarget and m_eventOrigin are not encoded as they are local only informations
     * @return
     */
    virtual void deserialize(const QByteArray &t_data) = 0;

    /**
     * @brief unique id of the entity concerning this event data
     */
    int entityId() const;
    void setEntityId(int t_entityId);

private:
    /**
     * @note this member variable is not serialized, as it is only valid in a local context
     */
    EventOrigin m_eventOrigin;

    /**
     * @note this member variable is not serialized, as it is only valid in a local context
     */
    EventTarget m_eventTarget;

    /**
     * @brief the id of the relevant entity
     */
    int m_entityId = -1;
};
} // namespace VeinEvent

#endif // VEINEVENT_VE_EVENTDATA_H

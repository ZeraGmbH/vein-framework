#ifndef OPAQUEDATA_H
#define OPAQUEDATA_H

#include "globalIncludes.h"
#include "ve_commandevent.h"
#include <ve_eventdata.h>
#include <memory>

namespace VeinComponent
{
class OpaqueCommandEvent : public VeinEvent::EventData
{
public:
    OpaqueCommandEvent(std::unique_ptr<VeinEvent::CommandEvent> cmdEvent, int opaqueId);

    int opaqueId() const;
    std::unique_ptr<VeinEvent::CommandEvent> dragCommandEvent();

    static constexpr int dataType() { return VCMP_OPAQUEDATA_DATATYPE; }
    bool isValid() const override;
    int type() const override { return VCMP_OPAQUEDATA_DATATYPE; }
    QByteArray serialize() const override;
    void deserialize(const QByteArray &t_data) override;
private:
    std::unique_ptr<VeinEvent::CommandEvent> m_cmdEvent;
    int m_opaqueId;
};
}
#endif // OPAQUEDATA_H

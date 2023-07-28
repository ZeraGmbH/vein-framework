#ifndef VEINCOMPONENT_INTROSPECTIONDATA_H
#define VEINCOMPONENT_INTROSPECTIONDATA_H

#include "vfcomponent_export.h"
#include "globalIncludes.h"
#include <ve_eventdata.h>
#include <QJsonObject>

namespace VeinComponent
{
// Introspection QEvent payload
class VFCOMPONENT_EXPORT IntrospectionData : public VeinEvent::EventData
{
public:
    IntrospectionData();
    QJsonObject jsonData() const;
    void setJsonData(const QJsonObject &t_jsonData);

    static constexpr int dataType() { return VCMP_INTROSPECTIONDATA_DATATYPE; }
    bool isValid() const override;
    int type() const override { return VCMP_INTROSPECTIONDATA_DATATYPE; }
    QByteArray serialize() const override;
    void deserialize(const QByteArray &t_data) override;
private:
    QJsonObject m_jsonData;
};
} // namespace VeinComponent

#endif // VEINCOMPONENT_INTROSPECTIONDATA_H

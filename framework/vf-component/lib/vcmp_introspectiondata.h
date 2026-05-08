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

    const QVariantMap &componentValues() const;
    void setComponentValues(const QVariantMap &componentValues);
    const QStringList &rpcNames() const;
    void setRpcNames(const QStringList &rpcNames);

    static constexpr int dataType() { return VCMP_INTROSPECTIONDATA_DATATYPE; }
    bool isValid() const override;
    int type() const override { return VCMP_INTROSPECTIONDATA_DATATYPE; }
    QByteArray serialize() const override;
    void deserialize(const QByteArray &data) override;
private:
    QVariantMap m_componentValues;
    QStringList m_rpcNames;
};
} // namespace VeinComponent

#endif // VEINCOMPONENT_INTROSPECTIONDATA_H

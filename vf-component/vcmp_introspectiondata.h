#ifndef VEINCOMPONENT_INTROSPECTIONDATA_H
#define VEINCOMPONENT_INTROSPECTIONDATA_H

#include "vfcomponent_export.h"
#include "globalIncludes.h"
#include <ve_eventdata.h>

#include <QJsonObject>

namespace VeinComponent
{
  /**
   * @brief Transports introspection information in Qt JSON format
   */
  class VFCOMPONENT_EXPORT IntrospectionData : public VeinEvent::EventData
  {
  public:
    IntrospectionData();
    QJsonObject jsonData() const;
    void setJsonData(const QJsonObject &t_jsonData);
    /**
     * @brief The dataType is a unique identifier for this type of EventData
     * @return
     */
    static constexpr int dataType() { return VCMP_INTROSPECTIONDATA_DATATYPE; }

    // EventData interface
  public:
    bool isValid() const override;
    int type() const override { return VCMP_INTROSPECTIONDATA_DATATYPE; }
    QByteArray serialize() const override;
    void deserialize(const QByteArray &t_data) override;

  private:
    /**
     * @brief the Qt JSON representation of the introspection
     */
    QJsonObject m_jsonData;
  };
} // namespace VeinComponent

#endif // VEINCOMPONENT_INTROSPECTIONDATA_H

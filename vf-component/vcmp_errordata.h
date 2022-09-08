#ifndef VEINCOMPONENT_ERRORDATA_H
#define VEINCOMPONENT_ERRORDATA_H

#include "vfcomponent_export.h"
#include "globalIncludes.h"
#include <ve_eventdata.h>


namespace VeinComponent
{
  /**
   * @brief Command object (see GoF Command pattern) for error informations
   */
  class VFCOMPONENT_EXPORT ErrorData : public VeinEvent::EventData
  {
  public:
    ErrorData();
    /**
     * @brief The dataType is a unique identifier for this type of EventData
     * @return
     */
    static constexpr int dataType() { return VCMP_ERRORDATA_DATATYPE; }

    int originalDataType() const;

    const QByteArray &originalData() const;
    void setOriginalData(EventData *t_originalData);

    QString errorDescription() const;
    void setErrorDescription(const QString &t_errorDescription);

    // EventData interface
  public:
    bool isValid() const override;
    int type() const override { return VCMP_ERRORDATA_DATATYPE; }
    QByteArray serialize() const override;
    void deserialize(const QByteArray &t_data) override;

  private:

    /**
     * @brief Used to deserialize the EventData
     * @sa EventData::type()
     */
    int m_originalDataType;

    /**
     * @brief The EventData that caused the error
     * @note do not cascade error data, e.g. do not send an error data event with error data event as content
     */
    QByteArray m_originalData;

    /**
     * @brief Human readable error string
     */
    QString m_errorDescription;

  };
} // namespace VeinComponent

#endif // VEINCOMPONENT_ERRORDATA_H

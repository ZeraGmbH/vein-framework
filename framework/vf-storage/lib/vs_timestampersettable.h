#ifndef VS_TIMESTAMPERSETTABLE_H
#define VS_TIMESTAMPERSETTABLE_H

#include "vs_abstracttimestamper.h"
#include <QObject>

namespace VeinStorage
{

class TimeStamperSettable;
typedef std::shared_ptr<TimeStamperSettable> TimeStamperSettablePtr;

class TimeStamperSettable : public QObject, public AbstractTimeStamper
{
    Q_OBJECT
public:
    static TimeStamperSettablePtr create();
    TimeStamperSettable();

    void setTimestampToNow();
    virtual QDateTime getTimestamp() override;
signals:
    void sigTimestampChanged();

private:
    QDateTime m_currentTimestamp;
};


}
#endif // VS_TIMESTAMPERSETTABLE_H

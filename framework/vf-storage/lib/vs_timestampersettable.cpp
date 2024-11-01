#include "vs_timestampersettable.h"
#include <timerfactoryqt.h>

namespace VeinStorage
{

TimeStamperSettablePtr TimeStamperSettable::create()
{
    return std::make_shared<TimeStamperSettable>();
}

TimeStamperSettable::TimeStamperSettable() :
    m_currentTimestamp(TimerFactoryQt::getCurrentTime())
{
}

void TimeStamperSettable::setTimestampToNow()
{
    QDateTime oldTimestamp = m_currentTimestamp;
    QDateTime newTimestamp = TimerFactoryQt::getCurrentTime();
    if(oldTimestamp.toMSecsSinceEpoch() != newTimestamp.toMSecsSinceEpoch()) {
        m_currentTimestamp = newTimestamp;
        emit sigTimestampChanged();
    }
}

QDateTime TimeStamperSettable::getTimestamp()
{
    return m_currentTimestamp;
}

}

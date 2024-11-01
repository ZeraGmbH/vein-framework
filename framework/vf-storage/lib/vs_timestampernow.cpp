#include "vs_timestampernow.h"
#include <timerfactoryqt.h>

namespace VeinStorage
{

AbstractTimeStamperPtr TimeStamperNow::m_instance;

AbstractTimeStamperPtr TimeStamperNow::getInstance()
{
    if(!m_instance)
        m_instance = std::make_shared<TimeStamperNow>();
    return m_instance;
}

QDateTime TimeStamperNow::getTimestamp()
{
    return TimerFactoryQt::getCurrentTime();
}

}

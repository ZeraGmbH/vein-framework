#ifndef TIMESTAMPERNOW_H
#define TIMESTAMPERNOW_H

#include "vs_abstracttimestamper.h"

namespace VeinStorage
{

class TimeStamperNow : public AbstractTimeStamper
{
public:
    static AbstractTimeStamperPtr getInstance();
    virtual QDateTime getTimestamp() override;
private:
    static AbstractTimeStamperPtr m_instance;
};

}
#endif // TIMESTAMPERNOW_H

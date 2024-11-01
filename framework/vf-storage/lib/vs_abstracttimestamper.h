#ifndef VS_ABSTRACTTIMESTAMPER_H
#define VS_ABSTRACTTIMESTAMPER_H

#include <QDateTime>
#include <memory>

namespace VeinStorage
{

class AbstractTimeStamper
{
public:
    virtual ~AbstractTimeStamper() = default;
    virtual QDateTime getTimestamp() = 0;
};
typedef std::shared_ptr<AbstractTimeStamper> AbstractTimeStamperPtr;

}

#endif // VS_ABSTRACTTIMESTAMPER_H

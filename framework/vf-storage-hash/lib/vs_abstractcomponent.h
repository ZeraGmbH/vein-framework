#ifndef ABSTRACTSTORAGECOMPONENT_H
#define ABSTRACTSTORAGECOMPONENT_H

#include "vfevent_export.h"
#include <QVariant>
#include <QDateTime>
#include <memory>

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractComponent : public QObject
{
    Q_OBJECT
signals:
    void sigValueSet(QVariant setValue);
    void sigValueChange(QVariant newValue);
public:
    virtual QVariant getValue() const = 0;
    virtual QDateTime getTimestamp() const = 0;
};

typedef std::shared_ptr<AbstractComponent> AbstractComponentPtr;

}
#endif // ABSTRACTSTORAGECOMPONENT_H

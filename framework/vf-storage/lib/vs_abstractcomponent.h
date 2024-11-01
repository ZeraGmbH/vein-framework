#ifndef VS_ABSTRACTCOMPONENT_H
#define VS_ABSTRACTCOMPONENT_H

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
};

typedef std::shared_ptr<AbstractComponent> AbstractComponentPtr;

}
#endif // VS_ABSTRACTCOMPONENT_H

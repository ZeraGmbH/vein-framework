#ifndef VS_ABSTRACTCOMPONENT_H
#define VS_ABSTRACTCOMPONENT_H

#include "vfevent_export.h"
#include "vs_abstractcomponentgetcustomizer.h"
#include <QVariant>
#include <QDateTime>
#include <memory>

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractComponent : public QObject
{
    Q_OBJECT
signals:
    void sigValueSet(const QVariant &setValue);
    void sigValueChange(const QVariant &newValue);
public:
    virtual const QVariant &getValue() const = 0;
    virtual void setValue(const QVariant &value) = 0;
    virtual void setGetValueCustomizer(AbstractComponentGetCustomizerPtr getCustomizer) = 0;
};

typedef std::shared_ptr<AbstractComponent> AbstractComponentPtr;
typedef QHash<QString, AbstractComponentPtr> EntityMap;

}
#endif // VS_ABSTRACTCOMPONENT_H

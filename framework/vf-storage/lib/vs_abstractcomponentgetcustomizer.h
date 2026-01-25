#ifndef VS_ABSTRACTCOMPONENTGETCUSTOMIZER_H
#define VS_ABSTRACTCOMPONENTGETCUSTOMIZER_H

#include "vfevent_export.h"
#include <QVariant>
#include <memory>

class VFEVENT_EXPORT AbstractComponentGetCustomizer
{
public:
    virtual const QVariant &getCustomizedValue(const QVariant &currentValue) = 0;
};

typedef std::shared_ptr<AbstractComponentGetCustomizer> AbstractComponentGetCustomizerPtr;

#endif // VS_ABSTRACTCOMPONENTGETCUSTOMIZER_H

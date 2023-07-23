#ifndef VFTESTCOMPONENTSPYFILTER_H
#define VFTESTCOMPONENTSPYFILTER_H

#include "vftestcomponentspy.h"

class VfTestComponentSpyFilter
{
public:
    // entityId
    static QList<VfTestComponentSpy::TComponentInfo> filter(QList<VfTestComponentSpy::TComponentInfo> componentList, int entityId);
    static bool hasOne(QList<VfTestComponentSpy::TComponentInfo> componentList, int entityId);
    static VfTestComponentSpy::TComponentInfo first(QList<VfTestComponentSpy::TComponentInfo> componentList, int entityId);

    // componentName
    static QList<VfTestComponentSpy::TComponentInfo> filter(QList<VfTestComponentSpy::TComponentInfo> componentList, QString componentName);
    static bool hasOne(QList<VfTestComponentSpy::TComponentInfo> componentList, QString componentName);
    static VfTestComponentSpy::TComponentInfo first(QList<VfTestComponentSpy::TComponentInfo> componentList, QString componentName);
};

#endif // VFTESTCOMPONENTSPYFILTER_H

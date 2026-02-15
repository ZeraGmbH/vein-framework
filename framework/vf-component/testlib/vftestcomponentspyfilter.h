#ifndef VFTESTCOMPONENTSPYFILTER_H
#define VFTESTCOMPONENTSPYFILTER_H

#include "vftestcomponentspy.h"

class VfTestComponentSpyFilter
{
public:
    // entityId
    static QList<VfTestComponentSpy::TComponentInfo> filter(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                                            int entityId);
    static bool hasOne(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                       int entityId);
    static VfTestComponentSpy::TComponentInfo first(const QList<VfTestComponentSpy::TComponentInfo> &componentList, int entityId);

    // componentName
    static QList<VfTestComponentSpy::TComponentInfo> filter(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                                            const QString &componentName);
    static bool hasOne(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                       const QString &componentName);
    static VfTestComponentSpy::TComponentInfo first(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                                    const QString &componentName);
};

#endif // VFTESTCOMPONENTSPYFILTER_H

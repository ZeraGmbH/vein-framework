#include "vftestcomponentspyfilter.h"

QList<VfTestComponentSpy::TComponentInfo> VfTestComponentSpyFilter::filter(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                                                           int entityId)
{
    QList<VfTestComponentSpy::TComponentInfo> found;
    for(const auto &component : componentList) {
        if(component.entityId == entityId)
            found.append(component);
    }
    return found;
}

bool VfTestComponentSpyFilter::hasOne(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                      int entityId)
{
    return filter(componentList, entityId).size() == 1;
}

VfTestComponentSpy::TComponentInfo VfTestComponentSpyFilter::first(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                                                   int entityId)
{
    QList<VfTestComponentSpy::TComponentInfo> list = filter(componentList, entityId);
    return list[0];
}

QList<VfTestComponentSpy::TComponentInfo> VfTestComponentSpyFilter::filter(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                                                           const QString &componentName)
{
    QList<VfTestComponentSpy::TComponentInfo> found;
    for(const auto &component : componentList) {
        if(component.componentName == componentName)
            found.append(component);
    }
    return found;
}

bool VfTestComponentSpyFilter::hasOne(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                      const QString &componentName)
{
    return filter(componentList, componentName).size() == 1;
}

VfTestComponentSpy::TComponentInfo VfTestComponentSpyFilter::first(const QList<VfTestComponentSpy::TComponentInfo> &componentList,
                                                                   const QString &componentName)
{
    QList<VfTestComponentSpy::TComponentInfo> list = filter(componentList, componentName);
    return list[0];
}

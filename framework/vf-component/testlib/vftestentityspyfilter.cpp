#include "vftestentityspyfilter.h"

QList<int> VfTestEntitySpyFilter::filter(const QList<int> &entityList, int entityId)
{
    QList<int> found;
    for(const auto &entity : entityList) {
        if(entity == entityId)
            found.append(entity);
    }
    return found;
}

bool VfTestEntitySpyFilter::hasOne(const QList<int> &entityList, int entityId)
{
    return filter(entityList, entityId).size() == 1;
}

int VfTestEntitySpyFilter::first(const QList<int> &entityList, int entityId)
{
    QList<int> list = filter(entityList, entityId);
    return list[0];
}

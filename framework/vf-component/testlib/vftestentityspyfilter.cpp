#include "vftestentityspyfilter.h"

QList<int> VfTestEntitySpyFilter::filter(QList<int> entityList, int entityId)
{
    QList<int> found;
    for(auto &entity : entityList) {
        if(entity == entityId)
            found.append(entity);
    }
    return found;
}

bool VfTestEntitySpyFilter::hasOne(QList<int> entityList, int entityId)
{
    return filter(entityList, entityId).size() == 1;
}

int VfTestEntitySpyFilter::first(QList<int> entityList, int entityId)
{
    QList<int> list = filter(entityList, entityId);
    return list[0];
}

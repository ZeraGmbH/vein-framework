#ifndef VFTESTENTITYSPYFILTER_H
#define VFTESTENTITYSPYFILTER_H

#include <QList>

class VfTestEntitySpyFilter
{
public:
    static QList<int> filter(QList<int> entityList, int entityId);
    static bool hasOne(QList<int> entityList, int entityId);
    static int first(QList<int> entityList, int entityId);
};

#endif // VFTESTENTITYSPYFILTER_H

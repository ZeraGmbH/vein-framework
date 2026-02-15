#ifndef VFTESTENTITYSPYFILTER_H
#define VFTESTENTITYSPYFILTER_H

#include <QList>

class VfTestEntitySpyFilter
{
public:
    static QList<int> filter(const QList<int> &entityList, int entityId);
    static bool hasOne(const QList<int> &entityList, int entityId);
    static int first(const QList<int> &entityList, int entityId);
};

#endif // VFTESTENTITYSPYFILTER_H

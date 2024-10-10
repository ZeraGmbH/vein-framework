#ifndef VF_SERVER_ENTITY_REMOVE_H
#define VF_SERVER_ENTITY_REMOVE_H

#include <QEvent>

class VfServerEntityRemove
{
public:
    static QEvent* generateEvent(int entityId);
};

#endif // VF_SERVER_ENTITY_REMOVE_H

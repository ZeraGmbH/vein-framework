#ifndef VF_SERVERENTITY_ADD_H
#define VF_SERVERENTITY_ADD_H

#include <QEvent>

class VfServerEntityAdd
{
public:
    static QEvent* generateEvent(int entityId);
};

#endif // VF_SERVERENTITY_ADD_H

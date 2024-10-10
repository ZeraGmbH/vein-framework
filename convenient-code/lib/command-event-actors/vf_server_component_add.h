#ifndef VF_SERVER_COMPONENT_ADD_H
#define VF_SERVER_COMPONENT_ADD_H

#include <QEvent>
#include <QVariant>

class VfServerComponentAdd
{
public:
    static QEvent *generateEvent(int entityId, QString componentName, QVariant initialValue);
};

#endif // VF_SERVER_COMPONENT_ADD_H

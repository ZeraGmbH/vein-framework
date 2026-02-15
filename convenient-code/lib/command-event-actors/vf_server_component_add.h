#ifndef VF_SERVER_COMPONENT_ADD_H
#define VF_SERVER_COMPONENT_ADD_H

#include <QEvent>
#include <QVariant>

class VfServerComponentAdd
{
public:
    static QEvent *generateEvent(int entityId, const QString &componentName, const QVariant &initialValue);
};

#endif // VF_SERVER_COMPONENT_ADD_H

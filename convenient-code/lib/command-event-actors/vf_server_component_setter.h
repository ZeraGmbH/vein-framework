#ifndef VFSERVERCOMPONENTSETTER_H
#define VFSERVERCOMPONENTSETTER_H

#include <QEvent>
#include <QVariant>

class VfServerComponentSetter
{
public:
    static QEvent *generateEvent(int entityId, QString componentName, QVariant oldValue, QVariant newValue);
};

#endif // VFSERVERCOMPONENTSETTER_H

#ifndef VFSERVERCOMPONENTSETTER_H
#define VFSERVERCOMPONENTSETTER_H

#include <QEvent>
#include <QVariant>

class VfServerComponentSetter
{
public:
    static QEvent *generateEvent(int entityId, const QString &componentName, const QVariant &oldValue, const QVariant &newValue);
};

#endif // VFSERVERCOMPONENTSETTER_H

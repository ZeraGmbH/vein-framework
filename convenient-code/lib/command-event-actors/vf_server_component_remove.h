#ifndef VFSERVERCOMPONENTREMOVE_H
#define VFSERVERCOMPONENTREMOVE_H

#include <QEvent>
#include <QString>

class VfServerComponentRemove
{
public:
    static QEvent *generateEvent(int entityId, QString componentName);
};

#endif // VFSERVERCOMPONENTREMOVE_H

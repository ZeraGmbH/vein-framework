#ifndef VFRPCEVENTGENERATOR_H
#define VFRPCEVENTGENERATOR_H

#include <QObject>

class VfRPCEventGenerator : public QObject
{
    Q_OBJECT
public:
    explicit VfRPCEventGenerator() {};
    virtual QEvent *generateEvent(int entityId, const QString &procedureName, const QVariantMap &parameters, const QUuid &identifier) = 0;
};

#endif // VFRPCEVENTGENERATOR_H

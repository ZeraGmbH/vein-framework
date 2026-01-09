#ifndef VS_ABSTRACTSTORAGESYSTEM_H
#define VS_ABSTRACTSTORAGESYSTEM_H

#include "vfevent_export.h"
#include "vs_abstractdatabase.h"
#include "ve_eventsystem.h"

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit AbstractEventSystem(QObject *parent = nullptr) : EventSystem(parent) {}

    virtual AbstractDatabase* getDb() const = 0;
    virtual QMap<int, QStringList> getRpcs() const = 0;
};
}

#endif // VS_ABSTRACTSTORAGESYSTEM_H


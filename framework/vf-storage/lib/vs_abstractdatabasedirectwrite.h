#ifndef VS_ABSTRACTDATABASEDIRECTWRITE_H
#define VS_ABSTRACTDATABASEDIRECTWRITE_H

#include "vs_abstractdatabase.h"
#include <QVariant>

namespace VeinStorage
{

class AbstractDatabaseDirectWrite : public AbstractDatabase
{
public:
    virtual StorageComponentPtr getFutureComponentForWrite(int entityId, const QString &componentName) = 0;
};

}
#endif // VS_ABSTRACTDATABASEDIRECTWRITE_H

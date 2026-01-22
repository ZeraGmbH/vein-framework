#ifndef VS_ABSTRACTSTORAGEWRITABLE_H
#define VS_ABSTRACTSTORAGEWRITABLE_H

#include "vfevent_export.h"
#include "vs_abstractdatabasedirectwrite.h"

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractStorageWritable
{
public:
    virtual AbstractDatabaseDirectWrite* getDbWritable() const = 0;
};
}

#endif // VS_ABSTRACTSTORAGEWRITABLE_H

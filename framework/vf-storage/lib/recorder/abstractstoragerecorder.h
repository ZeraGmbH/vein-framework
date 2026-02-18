#ifndef ABSTRACTSTORAGERECORDER_H
#define ABSTRACTSTORAGERECORDER_H

#include "vs_storagerecordertypes.h"
#include <QObject>

using namespace VeinStorage;

class AbstractStorageRecorder : public QObject
{
    Q_OBJECT
public:
    virtual StorageRecordDataPtr getRecordedData() const =0 ;
signals:
    void sigRecordCountChanged(int count);
};

#endif // ABSTRACTSTORAGERECORDER_H

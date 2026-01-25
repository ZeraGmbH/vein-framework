#ifndef STORAGEDATACOLLECTOR_H
#define STORAGEDATACOLLECTOR_H

#include "vs_storagerecordertypes.h"
#include "vs_abstractdatabase.h"
#include <QDateTime>

namespace VeinStorage {

class StorageRecorder : public QObject
{
    Q_OBJECT
public:
    explicit StorageRecorder(const RecordEntityComponents &componentsRecorded,
                             AbstractDatabase* storage,
                             int entityIdTrigger, const QString &componentTrigger, const QVariant &triggerVal = 1);

    void startLogging();
    void stopLogging();
    StorageRecordDataPtr getRecordedData() const;
signals:
    void sigRecordCountChanged(int count);

private slots:
    void onTrigger(const QVariant &value);
private:
    void setupVeinLoggedComponents(const RecordEntityComponents &componentsRecorded, AbstractDatabase *storage);
    RecordEntryStored createRecordEntry();

    std::shared_ptr<StorageRecordData> m_recordData;

    AbstractComponentPtr m_triggerComponent;
    const QVariant m_triggerVal;

    QList<AbstractComponentPtr> m_veinValuesRecorded;
};

}
#endif // STORAGEDATACOLLECTOR_H

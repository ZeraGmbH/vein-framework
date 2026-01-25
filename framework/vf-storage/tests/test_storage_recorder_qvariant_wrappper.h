#ifndef TEST_STORAGE_RECORDER_QVARIANT_WRAPPPER_H
#define TEST_STORAGE_RECORDER_QVARIANT_WRAPPPER_H

#include <QObject>

class test_storage_recorder_qvariant_wrappper : public QObject
{
    Q_OBJECT
private slots:
    void emptyMap();
    void emptyMapToWrapperChangeCopyVariant();
    void emptyMapInStorage();

    void emptyMapToStorageAddValueFetchVein();
};

#endif // TEST_STORAGE_RECORDER_QVARIANT_WRAPPPER_H

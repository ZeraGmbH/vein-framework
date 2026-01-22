#ifndef TEST_STORAGE_DIRECT_WRITE_H
#define TEST_STORAGE_DIRECT_WRITE_H

#include "vs_abstractdatabasedirectwrite.h"
#include "testveinserverwithmocknet.h"
#include "vf_core_stack_client.h"
#include "testjsonspyeventsystem.h"
#include <QObject>

using namespace VeinStorage;

class test_storage_direct_write : public QObject
{
    Q_OBJECT
private slots:
    void addFuturesChangeValue();
    void addFuturesChangeValueVeinAdd();
    void addFutureDumpIntrospection();
    void addFutureAddVeinDumpIntrospection();
    void addFutureFetch();
    void addFutureAddVeinFetch();

    // TODO
    // * introspection: ignore those directly added only OR send empty on post fetch?
    // * fetch test (vf-logger)
    // * SCPI meta only component

private:
    bool checkAllFutureGetters(AbstractDatabaseDirectWrite *writableDb, const QVariant &expectedValue);
    bool checkSameIdentities(AbstractDatabaseDirectWrite *writableDb, const StorageComponentPtr component);
    void setupSpy(TestVeinServerWithMockNet &serverNet, VfCoreStackClient &clientNet, QJsonObject &jsonEvents);

    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyBottom;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyBottom;
};

#endif // TEST_STORAGE_DIRECT_WRITE_H

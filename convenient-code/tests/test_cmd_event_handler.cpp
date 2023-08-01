#include "test_cmd_event_handler.h"
#include "vcmp_entitydata.h"
#include "vf_cmd_event_handler.h"
#include "vf_cmd_event_handler_item_test.h"
#include <vcmp_componentdata.h>
#include "vcmp_errordata.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_cmd_event_handler)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int dummyEntitiy = 0;
static constexpr CommandEvent::EventSubtype standardSubtype = CommandEvent::EventSubtype::NOTIFICATION;

void test_cmd_event_handler::notificationFilter()
{
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventHandlerItemTestPtr testItem = VfCmdEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, new ComponentData(dummyEntitiy));
    QSignalSpy spyNotify(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 1);
    delete commandNotify;

    CommandEvent *commandTransaction = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, new ComponentData(dummyEntitiy));
    QSignalSpy spyTransaction(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(commandTransaction);
    QCOMPARE(spyTransaction.count(), 0);
    delete commandTransaction;
}

void test_cmd_event_handler::transactionFilter()
{
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::TRANSACTION);
    VfCmdEventHandlerItemTestPtr testItem = VfCmdEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, new ComponentData(dummyEntitiy));
    QSignalSpy spyNotify(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 0);
    delete commandNotify;

    CommandEvent *commandTransaction = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, new ComponentData(dummyEntitiy));
    QSignalSpy spyTransaction(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(commandTransaction);
    QCOMPARE(spyTransaction.count(), 1);
    delete commandTransaction;
}

void test_cmd_event_handler::multipleEntities()
{
    VfCmdEventHandler commandEventHandler;
    VfCmdEventHandlerItemTestPtr testItem = VfCmdEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    CommandEvent *command1= new CommandEvent(standardSubtype, new ComponentData(dummyEntitiy));
    QSignalSpy spy1(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(command1);
    QCOMPARE(spy1.count(), 1);
    delete command1;

    CommandEvent *command2 = new CommandEvent(standardSubtype, new ComponentData(dummyEntitiy+1));
    QSignalSpy spy2(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(command2);
    QCOMPARE(spy2.count(), 0);
    delete command2;
}

void test_cmd_event_handler::multipleItems()
{
    VfCmdEventHandler commandEventHandler;
    VfCmdEventHandlerItemTestPtr testItem1 = VfCmdEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem1);
    VfCmdEventHandlerItemTestPtr testItem2 = VfCmdEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem2);

    CommandEvent *command1= new CommandEvent(standardSubtype, new ComponentData(dummyEntitiy));
    QSignalSpy spy1(testItem1.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(command1);
    QCOMPARE(spy1.count(), 1);
    delete command1;

    CommandEvent *command2 = new CommandEvent(standardSubtype, new ComponentData(dummyEntitiy));
    QSignalSpy spy2(testItem2.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(command2);
    QCOMPARE(spy2.count(), 1);
    delete command2;
}

void test_cmd_event_handler::unknownEventType()
{
    VfCmdEventHandler commandEventHandler;
    VfCmdEventHandlerItemTestPtr testItem = VfCmdEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    QEvent *unknownEvent = new QEvent(QEvent::ActionAdded);
    QSignalSpy spy(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(unknownEvent);
    QCOMPARE(spy.count(), 0);
    delete unknownEvent;
}

void test_cmd_event_handler::handleErrorEvents()
{
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventHandlerItemTestPtr testItem = VfCmdEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    ErrorData* errorData = new ErrorData;
    errorData->setEntityId(dummyEntitiy);
    errorData->setOriginalData(new ComponentData(dummyEntitiy));
    CommandEvent *errorEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, errorData);

    QSignalSpy spy(testItem.get(), &VfCmdEventHandlerItemTest::sigCommandErrorEventReceived);
    commandEventHandler.processEvent(errorEvent);
    delete errorEvent;

    QCOMPARE(spy.count(), 1);
}

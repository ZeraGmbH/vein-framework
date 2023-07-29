#include "test_command_event_handler.h"
#include "vfcommandeventhandler.h"
#include "vfcommandeventhandleritemtest.h"
#include <vcmp_componentdata.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_command_event_handler)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int dummyEntitiy = 0;

void test_command_event_handler::notificationFilter()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, new ComponentData(dummyEntitiy));
    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandNotify);
    commandEventHandler.processCommandEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 2);
    delete commandNotify;

    CommandEvent *commandTransaction = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, new ComponentData(dummyEntitiy));
    QSignalSpy spyTransaction(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandTransaction);
    commandEventHandler.processCommandEvent(commandTransaction);
    QCOMPARE(spyTransaction.count(), 0);
    delete commandTransaction;
}

void test_command_event_handler::transactionFilter()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::TRANSACTION);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, new ComponentData(dummyEntitiy));
    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandNotify);
    commandEventHandler.processCommandEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 0);
    delete commandNotify;

    CommandEvent *commandTransaction = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, new ComponentData(dummyEntitiy));
    QSignalSpy spyTransaction(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandTransaction);
    commandEventHandler.processCommandEvent(commandTransaction);
    QCOMPARE(spyTransaction.count(), 2);
    delete commandTransaction;
}

void test_command_event_handler::multipleEntities()
{
    constexpr CommandEvent::EventSubtype eventSubtype = CommandEvent::EventSubtype::NOTIFICATION;
    VfCommandEventHandler commandEventHandler(eventSubtype);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    CommandEvent *command1= new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy));
    QSignalSpy spy1(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(command1);
    commandEventHandler.processCommandEvent(command1);
    QCOMPARE(spy1.count(), 2);
    delete command1;

    CommandEvent *command2 = new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy+1));
    QSignalSpy spy2(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(command2);
    commandEventHandler.processCommandEvent(command2);
    QCOMPARE(spy2.count(), 0);
    delete command2;
}

void test_command_event_handler::multipleItems()
{
    constexpr CommandEvent::EventSubtype eventSubtype = CommandEvent::EventSubtype::NOTIFICATION;
    VfCommandEventHandler commandEventHandler(eventSubtype);
    VfCommandEventHandlerItemTestPtr testItem1 = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem1);
    VfCommandEventHandlerItemTestPtr testItem2 = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem2);

    CommandEvent *command1= new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy));
    QSignalSpy spy1(testItem1.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(command1);
    commandEventHandler.processCommandEvent(command1);
    QCOMPARE(spy1.count(), 2);
    delete command1;

    CommandEvent *command2 = new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy));
    QSignalSpy spy2(testItem2.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(command2);
    commandEventHandler.processCommandEvent(command2);
    QCOMPARE(spy2.count(), 2);
    delete command2;
}

void test_command_event_handler::unknownEventType()
{
    constexpr CommandEvent::EventSubtype eventSubtype = CommandEvent::EventSubtype::NOTIFICATION;
    VfCommandEventHandler commandEventHandler(eventSubtype);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    QEvent *unknownEvent = new QEvent(QEvent::ActionAdded);
    QSignalSpy spy(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(unknownEvent);
    QCOMPARE(spy.count(), 0);
    delete unknownEvent;
}


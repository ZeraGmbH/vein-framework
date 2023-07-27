#include "test_vfcommandeventhandler.h"
#include "vfcommandeventhandler.h"
#include "vfcommandeventhandleritemtest.h"
#include <vcmp_componentdata.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_vfcommandeventhandler)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int dummyEntitiy = 0;

void test_vfcommandeventhandler::notificationFilter()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, new ComponentData(dummyEntitiy));
    commandEventHandler.processEvent(commandNotify);
    commandEventHandler.processCommandEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 2);

    QSignalSpy spyTransaction(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *commandTransaction = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, new ComponentData(dummyEntitiy));
    commandEventHandler.processEvent(commandTransaction);
    commandEventHandler.processCommandEvent(commandTransaction);
    QCOMPARE(spyTransaction.count(), 0);
}

void test_vfcommandeventhandler::transactionFilter()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::TRANSACTION);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, new ComponentData(dummyEntitiy));
    commandEventHandler.processEvent(commandNotify);
    commandEventHandler.processCommandEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 0);

    QSignalSpy spyTransaction(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *commandTransaction = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, new ComponentData(dummyEntitiy));
    commandEventHandler.processEvent(commandTransaction);
    commandEventHandler.processCommandEvent(commandTransaction);
    QCOMPARE(spyTransaction.count(), 2);
}

void test_vfcommandeventhandler::multipleEntities()
{
    constexpr CommandEvent::EventSubtype eventSubtype = CommandEvent::EventSubtype::NOTIFICATION;
    VfCommandEventHandler commandEventHandler(eventSubtype);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    QSignalSpy spy1(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *command1= new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy));
    commandEventHandler.processEvent(command1);
    commandEventHandler.processCommandEvent(command1);
    QCOMPARE(spy1.count(), 2);

    QSignalSpy spy2(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *command2 = new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy+1));
    commandEventHandler.processEvent(command2);
    commandEventHandler.processCommandEvent(command2);
    QCOMPARE(spy2.count(), 0);
}

void test_vfcommandeventhandler::multipleItems()
{
    constexpr CommandEvent::EventSubtype eventSubtype = CommandEvent::EventSubtype::NOTIFICATION;
    VfCommandEventHandler commandEventHandler(eventSubtype);
    VfCommandEventHandlerItemTestPtr testItem1 = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem1);
    VfCommandEventHandlerItemTestPtr testItem2 = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem2);

    QSignalSpy spy1(testItem1.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *command1= new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy));
    commandEventHandler.processEvent(command1);
    commandEventHandler.processCommandEvent(command1);
    QCOMPARE(spy1.count(), 2);

    QSignalSpy spy2(testItem2.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    CommandEvent *command2 = new CommandEvent(eventSubtype, new ComponentData(dummyEntitiy));
    commandEventHandler.processEvent(command2);
    commandEventHandler.processCommandEvent(command2);
    QCOMPARE(spy2.count(), 2);
}

void test_vfcommandeventhandler::unknownEventType()
{
    constexpr CommandEvent::EventSubtype eventSubtype = CommandEvent::EventSubtype::NOTIFICATION;
    VfCommandEventHandler commandEventHandler(eventSubtype);
    VfCommandEventHandlerItemTestPtr testItem = VfCommandEventHandlerItemTest::create(dummyEntitiy);
    commandEventHandler.addItem(testItem);

    QSignalSpy spy(testItem.get(), &VfCommandEventHandlerItemTest::sigCommandEvenProcessed);
    QEvent *unknownEvent = new QEvent(QEvent::ActionAdded);
    commandEventHandler.processEvent(unknownEvent);
    QCOMPARE(spy.count(), 0);
}


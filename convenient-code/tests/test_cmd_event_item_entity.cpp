#include "test_cmd_event_item_entity.h"
#include "vf_cmd_event_handler.h"
#include "vf_cmd_event_handler_component_item_test.h"
#include <vcmp_componentdata.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_cmd_event_item_entity)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int entitiyId1 = 0;
static constexpr int entitiyId2 = 1;
static const char* componentName1 = "foo";
static const char* componentName2 = "bar";

void test_cmd_event_item_entity::ignoreOtherComponentSameEntity()
{
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(entitiyId1);
    commandEventHandler.addItem(entityItem);
    VfCmdEventHandlerComponentItemTestPtr testItem = VfCmdEventHandlerComponentItemTest::create(componentName1, entityItem);
    entityItem->addItem(testItem);

    ComponentData* cData = new ComponentData(entitiyId1);
    cData->setComponentName(componentName2);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);

    QSignalSpy spyNotify(testItem.get(), &VfCmdEventHandlerComponentItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 0);
    delete commandNotify;
}

void test_cmd_event_item_entity::acceptSameComponentSameEntity()
{
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(entitiyId1);
    commandEventHandler.addItem(entityItem);
    VfCmdEventHandlerComponentItemTestPtr testItem = VfCmdEventHandlerComponentItemTest::create(componentName1, entityItem);
    entityItem->addItem(testItem);

    ComponentData* cData = new ComponentData(entitiyId1);
    cData->setComponentName(componentName1);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);

    QSignalSpy spyNotify(testItem.get(), &VfCmdEventHandlerComponentItemTest::sigCommandEventReceived);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 1);
    delete commandNotify;
}

void test_cmd_event_item_entity::sameComponentNamesOnDifferentEntities()
{
    const char* commonComponentName = "commonFoo";
    VfCmdEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem1 = VfCmdEventItemEntity::create(entitiyId1);
    VfCmdEventItemEntityPtr entityItem2 = VfCmdEventItemEntity::create(entitiyId2);
    commandEventHandler.addItem(entityItem1);
    commandEventHandler.addItem(entityItem2);
    VfCmdEventHandlerComponentItemTestPtr testItem1 = VfCmdEventHandlerComponentItemTest::create(commonComponentName, entityItem1);
    VfCmdEventHandlerComponentItemTestPtr testItem2 = VfCmdEventHandlerComponentItemTest::create(commonComponentName, entityItem2);
    entityItem1->addItem(testItem1);
    entityItem2->addItem(testItem2);

    QSignalSpy spyNotify1(testItem1.get(), &VfCmdEventHandlerComponentItemTest::sigCommandEventReceived);
    QSignalSpy spyNotify2(testItem2.get(), &VfCmdEventHandlerComponentItemTest::sigCommandEventReceived);

    ComponentData* cData1 = new ComponentData(entitiyId1);
    cData1->setComponentName(commonComponentName);
    CommandEvent *commandNotify1 = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData1);
    commandEventHandler.processEvent(commandNotify1);
    QCOMPARE(spyNotify1.count(), 1);
    QCOMPARE(spyNotify2.count(), 0);

    ComponentData* cData2 = new ComponentData(entitiyId2);
    cData2->setComponentName(commonComponentName);
    CommandEvent *commandNotify2 = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData2);
    commandEventHandler.processEvent(commandNotify2);
    QCOMPARE(spyNotify1.count(), 1);
    QCOMPARE(spyNotify2.count(), 1);

    delete commandNotify1;
    delete commandNotify2;
}

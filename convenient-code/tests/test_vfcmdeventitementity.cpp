#include "test_vfcmdeventitementity.h"
#include "vfcommandeventhandler.h"
#include "vfcommandeventhandlercomponentitemtest.h"
#include <vcmp_componentdata.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_vfcmdeventitementity)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int entitiyId1 = 0;
static constexpr int entitiyId2 = 1;
static const char* componentName1 = "foo";
static const char* componentName2 = "bar";

void test_vfcmdeventitementity::ignoreOtherComponentSameEntity()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(entitiyId1);
    commandEventHandler.addItem(entityItem);
    VfCommandEventHandlerComponentItemTestPtr testItem = VfCommandEventHandlerComponentItemTest::create(componentName1, entityItem);
    entityItem->addItem(testItem);

    ComponentData* cData = new ComponentData(entitiyId1);
    cData->setComponentName(componentName2);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);

    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerComponentItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 0);
    delete commandNotify;
}

void test_vfcmdeventitementity::acceptSameComponentSameEntity()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(entitiyId1);
    commandEventHandler.addItem(entityItem);
    VfCommandEventHandlerComponentItemTestPtr testItem = VfCommandEventHandlerComponentItemTest::create(componentName1, entityItem);
    entityItem->addItem(testItem);

    ComponentData* cData = new ComponentData(entitiyId1);
    cData->setComponentName(componentName1);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);

    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerComponentItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 1);
    delete commandNotify;
}

void test_vfcmdeventitementity::sameComponentNamesOnDifferentEntities()
{
    const char* commonComponentName = "commonFoo";
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventItemEntityPtr entityItem1 = VfCmdEventItemEntity::create(entitiyId1);
    VfCmdEventItemEntityPtr entityItem2 = VfCmdEventItemEntity::create(entitiyId2);
    commandEventHandler.addItem(entityItem1);
    commandEventHandler.addItem(entityItem2);
    VfCommandEventHandlerComponentItemTestPtr testItem1 = VfCommandEventHandlerComponentItemTest::create(commonComponentName, entityItem1);
    VfCommandEventHandlerComponentItemTestPtr testItem2 = VfCommandEventHandlerComponentItemTest::create(commonComponentName, entityItem2);
    entityItem1->addItem(testItem1);
    entityItem2->addItem(testItem2);

    QSignalSpy spyNotify1(testItem1.get(), &VfCommandEventHandlerComponentItemTest::sigCommandEvenProcessed);
    QSignalSpy spyNotify2(testItem2.get(), &VfCommandEventHandlerComponentItemTest::sigCommandEvenProcessed);

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

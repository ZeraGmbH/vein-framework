#include "test_componentitem.h"
#include "vfcommandeventhandler.h"
#include "vfcommandeventhandlercomponentitemtest.h"
#include <vcmp_componentdata.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_componentitem)

using namespace VeinEvent;
using namespace VeinComponent;

static constexpr int dummyEntitiy = 0;
static const char* componentName1 = "foo";
static const char* componentName2 = "bar";

void test_componentitem::ignoreOtherComponent()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCommandEventHandlerComponentItemTestPtr testItem = VfCommandEventHandlerComponentItemTest::create(dummyEntitiy, componentName1);
    commandEventHandler.addItem(testItem);

    ComponentData* cData = new ComponentData(dummyEntitiy);
    cData->setComponentName(componentName2);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);

    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerComponentItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 0);
    delete commandNotify;
}

void test_componentitem::acceptSameComponent()
{
    VfCommandEventHandler commandEventHandler(CommandEvent::EventSubtype::NOTIFICATION);
    VfCommandEventHandlerComponentItemTestPtr testItem = VfCommandEventHandlerComponentItemTest::create(dummyEntitiy, componentName1);
    commandEventHandler.addItem(testItem);

    ComponentData* cData = new ComponentData(dummyEntitiy);
    cData->setComponentName(componentName1);
    CommandEvent *commandNotify = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);

    QSignalSpy spyNotify(testItem.get(), &VfCommandEventHandlerComponentItemTest::sigCommandEvenProcessed);
    commandEventHandler.processEvent(commandNotify);
    QCOMPARE(spyNotify.count(), 1);
    delete commandNotify;
}

#include "test_storage_future_notification.h"
#include "testveinserver.h"
#include "vs_storageeventsystem.h"
#include "vf_server_component_setter.cpp"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_storage_future_notification)

using namespace VeinStorage;

void test_storage_future_notification::getComponentOnEmptyHash()
{
    StorageEventSystem storage;
    AbstractComponentPtr component = storage.getDb()->getFutureComponent(1, "foo");

    QVERIFY(!component->getValue().isValid());
}

const int testEntityId = 10;

void test_storage_future_notification::getComponentAddedLater()
{
    TestVeinServer server;
    TimeMachineObject::feedEventLoop();

    AbstractEventSystem* storage = server.getStorage();
    AbstractComponentPtr component = storage->getDb()->getFutureComponent(testEntityId, "ComponentName1");

    QVERIFY(!component->getValue().isValid());

    server.addTestEntities();
    QEvent *event = VfServerComponentSetter::generateEvent(testEntityId, "ComponentName1", QVariant(), 42);
    emit storage->sigSendEvent(event);
    TimeMachineObject::feedEventLoop();

    QVERIFY(component->getValue().isValid());
    QCOMPARE(component->getValue(), 42);

    QVERIFY(storage->getDb()->areFutureComponentsEmpty());
}

void test_storage_future_notification::getComponentsAddedLater()
{
    TestVeinServer server;
    TimeMachineObject::feedEventLoop();

    AbstractEventSystem* storage = server.getStorage();
    AbstractComponentPtr component1 = storage->getDb()->getFutureComponent(testEntityId, "ComponentName1");
    AbstractComponentPtr component2 = storage->getDb()->getFutureComponent(testEntityId, "ComponentName2");

    QVERIFY(!component1->getValue().isValid());
    QVERIFY(!component2->getValue().isValid());
    QVERIFY(component1 != component2);

    server.addTestEntities();
    QEvent *event = VfServerComponentSetter::generateEvent(testEntityId, "ComponentName1", QVariant(), 42);
    emit storage->sigSendEvent(event);
    event = VfServerComponentSetter::generateEvent(testEntityId, "ComponentName2", QVariant(), 37);
    emit storage->sigSendEvent(event);
    TimeMachineObject::feedEventLoop();

    QVERIFY(component1->getValue().isValid());
    QCOMPARE(component1->getValue(), 42);

    QVERIFY(component2->getValue().isValid());
    QCOMPARE(component2->getValue(), 37);

    QVERIFY(storage->getDb()->areFutureComponentsEmpty());
}

void test_storage_future_notification::getComponentAddedLaterIdentity()
{
    TestVeinServer server;
    TimeMachineObject::feedEventLoop();
    AbstractEventSystem* storage = server.getStorage();

    AbstractComponentPtr futureComponent = storage->getDb()->getFutureComponent(testEntityId, "ComponentName1");

    server.addTestEntities();
    QEvent *event = VfServerComponentSetter::generateEvent(testEntityId, "ComponentName1", QVariant(), 42);
    emit storage->sigSendEvent(event);
    TimeMachineObject::feedEventLoop();

    AbstractComponentPtr component = storage->getDb()->findComponent(testEntityId, "ComponentName1");
    QCOMPARE(futureComponent, component);
}

void test_storage_future_notification::getFutureComponentAlreadyStored()
{
    TestVeinServer server;
    TimeMachineObject::feedEventLoop();
    AbstractEventSystem* storage = server.getStorage();

    server.addTestEntities();
    QEvent *event = VfServerComponentSetter::generateEvent(testEntityId, "ComponentName1", QVariant(), 42);
    emit storage->sigSendEvent(event);
    TimeMachineObject::feedEventLoop();

    AbstractComponentPtr futureComponent = storage->getDb()->getFutureComponent(testEntityId, "ComponentName1");

    QVERIFY(futureComponent->getValue().isValid());
    QCOMPARE(futureComponent->getValue(), 42);

    QVERIFY(storage->getDb()->areFutureComponentsEmpty());

    AbstractComponentPtr component = storage->getDb()->findComponent(testEntityId, "ComponentName1");
    QCOMPARE(futureComponent, component);
}

void test_storage_future_notification::checkChangeSignals()
{
    TestVeinServer server;
    TimeMachineObject::feedEventLoop();

    AbstractEventSystem* storage = server.getStorage();
    AbstractComponentPtr component = storage->getDb()->getFutureComponent(testEntityId, "ComponentName1");
    QSignalSpy spy(component.get(), &AbstractComponent::sigValueChange);

    // addTestEntities sets QVariant() so addEntity + addComponent
    server.addEntity(testEntityId, "TestEntityName");
    QCOMPARE(spy.count(), 0);

    server.addComponent(testEntityId, "ComponentName1", 42, false);
    QCOMPARE(spy.count(), 1);

    emit storage->sigSendEvent( VfServerComponentSetter::generateEvent(testEntityId, "ComponentName1", QVariant(), 37) );
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spy.count(), 2);
}

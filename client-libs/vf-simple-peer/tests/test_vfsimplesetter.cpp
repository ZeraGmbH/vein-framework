#include "test_vfsimplesetter.h"
#include "vfsimplesetter.h"
#include "vfcommandeventhandlersystem.h"
#include "veintestserver.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_vfsimplesetter)

static constexpr int systemEntityId = 0;
static constexpr int invalidId = 1;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet() :
        server(&eventHandler)
    {
        eventHandler.addSubsystem(&cmdEventHandlerSystem);
    }
};

void test_vfsimplesetter::setInvalidIsEvil()
{
    ServerNoNet server;
    feedEventLoop();

    VfSimpleSetterPtr setter = VfSimpleSetter::create(systemEntityId, "foo");
    server.cmdEventHandlerSystem.addItem(setter);

    setter->startSetComponent(QVariant(), QVariant());
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfSimpleSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
}

void test_vfsimplesetter::setEqualEmitsOk()
{
    ServerNoNet server;
    feedEventLoop();

    VfSimpleSetterPtr setter = VfSimpleSetter::create(systemEntityId, "foo");
    server.cmdEventHandlerSystem.addItem(setter);

    setter->startSetComponent("foo", "foo");
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfSimpleSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
}

void test_vfsimplesetter::setToInvalidEntity()
{
/*    ServerNoNet server;
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfSimpleSetterPtr setter = VfSimpleSetter::create(invalidId, "foo");
    server.cmdEventHandlerSystem.addItem(setter);
    // check event loop fired: connect after start
    setter->startSetComponent("foo", "bar");
    QSignalSpy spy(setter.get(), &VfSimpleSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());*/
}

void test_vfsimplesetter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}

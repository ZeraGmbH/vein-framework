#include "test_vf_cpp_client.h"
#include "modulemanagercontroller.h"
#include "ve_eventhandler.h"
#include "vf-cpp-entity.h"
#include "vftestcomponentaddlistener.h"
#include "vftestcomponentchangelistener.h"
#include "vn_introspectionsystem.h"
#include "vs_veinhash.h"
#include <QAbstractEventDispatcher>
#include <QTest>

QTEST_MAIN(test_vf_cpp_client)

void test_vf_cpp_client::init()
{
}

void test_vf_cpp_client::introspectionTest()
{
}

void test_vf_cpp_client::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}

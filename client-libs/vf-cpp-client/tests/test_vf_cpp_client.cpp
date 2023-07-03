#include "test_vf_cpp_client.h"
#include "ve_eventhandler.h"
#include "vf-cpp-entity.h"
#include <QTest>

QTEST_MAIN(test_vf_cpp_client)

void test_vf_cpp_client::init()
{
}

void test_vf_cpp_client::introspectionTest()
{
    VeinEvent::EventHandler vfEventHandler;
    VfCpp::veinmoduleentity systemEntity(0);
}

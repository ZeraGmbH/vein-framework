#include "test_container_safe_delete_while_loop.h"
#include "containersafedeletewhileloop.h"
#include <QTest>

QTEST_MAIN(test_container_safe_delete_while_loop)

void test_container_safe_delete_while_loop::emptyReturnsDefaultValue()
{
    ContainerSafeDeleteWhileLoop<void*> cont;
    QSet<void*> set;
    for(auto p = cont.getFirst(); p!=nullptr; p = cont.getNext())
        set.insert(p);
    QCOMPARE(set.count(), 0);
}

void test_container_safe_delete_while_loop::removeOnEmpty()
{
    // Just crash (throw) test
    ContainerSafeDeleteWhileLoop<int> cont;
    cont.removeItem(0);
}

void test_container_safe_delete_while_loop::addAndIterate()
{
    ContainerSafeDeleteWhileLoop<int> cont;
    cont.addItem(1);
    cont.addItem(2);
    cont.addItem(3);

    QSet<int> set;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        set.insert(i);
    QCOMPARE(set.count(), 3);
    QVERIFY(set.contains(1));
    QVERIFY(set.contains(2));
    QVERIFY(set.contains(3));
}

void test_container_safe_delete_while_loop::addRemoveAndIterate()
{
    ContainerSafeDeleteWhileLoop<int> cont;
    cont.addItem(1);
    cont.addItem(2);
    cont.addItem(3);
    cont.removeItem(2);
    QSet<int> set;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        set.insert(i);
    QCOMPARE(set.count(), 2);
    QVERIFY(set.contains(1));
    QVERIFY(set.contains(3));
}

void test_container_safe_delete_while_loop::deleteAllOnFirstIteration()
{
    ContainerSafeDeleteWhileLoop<int> cont;
    cont.addItem(1);
    cont.addItem(2);
    cont.addItem(3);
    QSet<int> setloop;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext()) {
        cont.removeItem(1);
        cont.removeItem(2);
        cont.removeItem(3);
        setloop.insert(i);
    }
    QCOMPARE(setloop.count(), 1);

    QSet<int> stillThereSet;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        stillThereSet.insert(i);
    QCOMPARE(stillThereSet.count(), 0);
}

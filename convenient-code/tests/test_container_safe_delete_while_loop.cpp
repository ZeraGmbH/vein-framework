#include "test_container_safe_delete_while_loop.h"
#include "container_safe_delete_while_loop.h"
#include <set>
#include <QTest>

QTEST_MAIN(test_container_safe_delete_while_loop)

void test_container_safe_delete_while_loop::emptyReturnsDefaultValueUnordered()
{
    ContainerSafeDeleteWhileLoop<void*, std::unordered_set<void*>> container;
    QSet<void*> resultSet;
    for(auto p = container.getFirst(); p!=nullptr; p = container.getNext())
        resultSet.insert(p);
    QCOMPARE(resultSet.count(), 0);
}

void test_container_safe_delete_while_loop::emptyReturnsDefaultValueOrdered()
{
    ContainerSafeDeleteWhileLoop<void*, std::set<void*>> container;
    QSet<void*> resultSet;
    for(auto p = container.getFirst(); p!=nullptr; p = container.getNext())
        resultSet.insert(p);
    QCOMPARE(resultSet.count(), 0);
}

void test_container_safe_delete_while_loop::removeOnEmptyUnordered()
{
    // Just crash (throw) test
    ContainerSafeDeleteWhileLoop<int, std::unordered_set<int>> cont;
    cont.removeItem(0);
}

void test_container_safe_delete_while_loop::removeOnEmptyOrdered()
{
    // Just crash (throw) test
    ContainerSafeDeleteWhileLoop<int, std::set<int>> cont;
    cont.removeItem(0);
}

void test_container_safe_delete_while_loop::addAndIterateUnordered()
{
    ContainerSafeDeleteWhileLoop<int, std::unordered_set<int>> cont;
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

void test_container_safe_delete_while_loop::addAndIterateOrdered()
{
    ContainerSafeDeleteWhileLoop<int, std::set<int>> cont;
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

void test_container_safe_delete_while_loop::addRemoveAndIterateUnordered()
{
    ContainerSafeDeleteWhileLoop<int, std::unordered_set<int>> cont;
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

void test_container_safe_delete_while_loop::addRemoveAndIterateOrdered()
{
    ContainerSafeDeleteWhileLoop<int, std::set<int>> cont;
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

void test_container_safe_delete_while_loop::deleteFirstOnFirstIterationUnordered()
{
    ContainerSafeDeleteWhileLoop<int, std::unordered_set<int>> cont;
    cont.addItem(1);
    cont.addItem(2);
    cont.addItem(3);
    // we don't know the sequeence so just check what's stoll there
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext()) {
        if(i==1)
            cont.removeItem(1);
    }

    QSet<int> stillThereSet;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        stillThereSet.insert(i);
    QCOMPARE(stillThereSet.count(), 2);
    QVERIFY(stillThereSet.contains(2));
    QVERIFY(stillThereSet.contains(3));
}

void test_container_safe_delete_while_loop::deleteFirstOnFirstIterationOrdered()
{
    ContainerSafeDeleteWhileLoop<int, std::set<int>> cont;
    cont.addItem(1);
    cont.addItem(2);
    cont.addItem(3);
    QSet<int> setloop;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext()) {
        if(i==1)
            cont.removeItem(1);
        setloop.insert(i);
    }
    QCOMPARE(setloop.count(), 3);
    QVERIFY(setloop.contains(1));
    QVERIFY(setloop.contains(2));
    QVERIFY(setloop.contains(3));

    QSet<int> stillThereSet;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        stillThereSet.insert(i);
    QCOMPARE(stillThereSet.count(), 2);
    QVERIFY(stillThereSet.contains(2));
    QVERIFY(stillThereSet.contains(3));
}

void test_container_safe_delete_while_loop::deleteSecondOnFirstIterationUnordered()
{
    ContainerSafeDeleteWhileLoop<int, std::unordered_set<int>> cont;
    cont.addItem(1);
    cont.addItem(2);
    cont.addItem(3);
    // we don't know the sequeence so just check what's stoll there
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext()) {
        if(i==1)
            cont.removeItem(2);
    }

    QSet<int> stillThereSet;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        stillThereSet.insert(i);
    QCOMPARE(stillThereSet.count(), 2);
    QVERIFY(stillThereSet.contains(1));
    QVERIFY(stillThereSet.contains(3));
}

void test_container_safe_delete_while_loop::deleteSecondOnFirstIterationOrdered()
{
    ContainerSafeDeleteWhileLoop<int, std::set<int>> cont;
    cont.addItem(1);
    cont.addItem(2);
    cont.addItem(3);
    QSet<int> setloop;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext()) {
        if(i==1)
            cont.removeItem(2);
        setloop.insert(i);
    }
    QCOMPARE(setloop.count(), 2);
    QVERIFY(setloop.contains(1));
    QVERIFY(setloop.contains(3));

    QSet<int> stillThereSet;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        stillThereSet.insert(i);
    QCOMPARE(stillThereSet.count(), 2);
    QVERIFY(stillThereSet.contains(1));
    QVERIFY(stillThereSet.contains(3));
}

void test_container_safe_delete_while_loop::deleteAllOnFirstIterationUnordered()
{
    ContainerSafeDeleteWhileLoop<int, std::unordered_set<int>> cont;
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

void test_container_safe_delete_while_loop::deleteAllOnFirstIterationOrdered()
{
    ContainerSafeDeleteWhileLoop<int, std::set<int>> cont;
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
    QVERIFY(setloop.contains(1));

    QSet<int> stillThereSet;
    for(auto i = cont.getFirst(); i!=0; i=cont.getNext())
        stillThereSet.insert(i);
    QCOMPARE(stillThereSet.count(), 0);
}

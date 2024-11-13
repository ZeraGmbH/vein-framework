#include "test_entity_dictionary.h"
#include "entitydictionary.h"
#include <QTest>

using namespace VeinApiQml;

QTEST_MAIN(test_entity_dictionary)

void test_entity_dictionary::findsOnEmpty()
{
    EntityDictionary dict;
    QCOMPARE(dict.findById(0), nullptr);
    QCOMPARE(dict.findByName("foo"), nullptr);
    QCOMPARE(dict.remove(0), nullptr);
}

void test_entity_dictionary::insertOneAndFindById()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    QVERIFY(dict.insert(entityId, map));

    EntityComponentMap* foundMap = dict.findById(entityId);
    QCOMPARE(foundMap, map);

    foundMap = dict.findById(42);
    QCOMPARE(foundMap, nullptr);
}

void test_entity_dictionary::insertTwoSameId()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    QVERIFY(dict.insert(entityId, map));
    QVERIFY(!dict.insert(entityId, map));
}

void test_entity_dictionary::insertTwoAndFind()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId0 = 0;
    EntityComponentMap* map0 = new EntityComponentMap(entityId0, dummyHash);
    QVERIFY(dict.insert(entityId0, map0));
    const int entityId1 = 1;
    EntityComponentMap* map1 = new EntityComponentMap(entityId1, dummyHash);
    QVERIFY(dict.insert(entityId1, map1));

    EntityComponentMap* foundMap0 = dict.findById(entityId0);
    QCOMPARE(foundMap0, map0);
    EntityComponentMap* foundMap1 = dict.findById(entityId1);
    QCOMPARE(foundMap1, map1);

    EntityComponentMap* foundMap = dict.findById(42);
    QCOMPARE(foundMap, nullptr);
}

void test_entity_dictionary::insertOneAndDelete()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    QVERIFY(dict.insert(entityId, map));

    EntityComponentMap* foundMap = dict.findById(42);
    QCOMPARE(foundMap, nullptr);

    foundMap = dict.remove(entityId);
    QCOMPARE(foundMap, map);

    foundMap = dict.findById(entityId);
    QCOMPARE(foundMap, nullptr);
}

void test_entity_dictionary::insertOneSetName()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    dict.insert(entityId, map);

    QVERIFY(dict.setEntityName(entityId, "foo"));
}

void test_entity_dictionary::insertOneSetEmptyName()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    dict.insert(entityId, map);

    QVERIFY(!dict.setEntityName(entityId, ""));
}

void test_entity_dictionary::insertOneSetSameNameTwice()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    dict.insert(entityId, map);

    QVERIFY(dict.setEntityName(entityId, "foo"));
    QVERIFY(dict.setEntityName(entityId, "foo"));
}

void test_entity_dictionary::insertOneSetNameInvalidId()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    dict.insert(entityId, map);

    QVERIFY(!dict.setEntityName(42, "foo"));
}

void test_entity_dictionary::insertOneSetNameFind()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    dict.insert(entityId, map);

    QVERIFY(dict.setEntityName(entityId, "foo"));
    EntityComponentMap* foundMap = dict.findByName("foo");
    QCOMPARE(foundMap, map);
}

void test_entity_dictionary::insertOneSetTwoNames()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    dict.insert(entityId, map);

    QVERIFY(dict.setEntityName(entityId, "foo"));
    QVERIFY(!dict.setEntityName(entityId, "bar"));
    EntityComponentMap* foundMap = dict.findByName("foo");
    QCOMPARE(foundMap, map);
}

void test_entity_dictionary::insertTwoSetTwoNames()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId0 = 0;
    EntityComponentMap* map0 = new EntityComponentMap(entityId0, dummyHash);
    QVERIFY(dict.insert(entityId0, map0));
    const int entityId1 = 1;
    EntityComponentMap* map1 = new EntityComponentMap(entityId1, dummyHash);
    QVERIFY(dict.insert(entityId1, map1));

    QVERIFY(dict.setEntityName(entityId0, "foo"));
    QVERIFY(dict.setEntityName(entityId1, "bar"));

    EntityComponentMap* foundMap = dict.findByName("foo");
    QCOMPARE(foundMap, map0);
    foundMap = dict.findByName("bar");
    QCOMPARE(foundMap, map1);
}

void test_entity_dictionary::insertTwoSetTwoNamesNotUnique()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId0 = 0;
    EntityComponentMap* map0 = new EntityComponentMap(entityId0, dummyHash);
    QVERIFY(dict.insert(entityId0, map0));
    const int entityId1 = 1;
    EntityComponentMap* map1 = new EntityComponentMap(entityId1, dummyHash);
    QVERIFY(dict.insert(entityId1, map1));

    QVERIFY(dict.setEntityName(entityId0, "foo"));
    QVERIFY(!dict.setEntityName(entityId1, "foo"));
}

void test_entity_dictionary::insertTwoSetFourNames()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId0 = 0;
    EntityComponentMap* map0 = new EntityComponentMap(entityId0, dummyHash);
    QVERIFY(dict.insert(entityId0, map0));
    const int entityId1 = 1;
    EntityComponentMap* map1 = new EntityComponentMap(entityId1, dummyHash);
    QVERIFY(dict.insert(entityId1, map1));

    QVERIFY(dict.setEntityName(entityId0, "foo"));
    QVERIFY(!dict.setEntityName(entityId0, "bar"));
    QVERIFY(dict.setEntityName(entityId1, "bar"));
    QVERIFY(!dict.setEntityName(entityId1, "baz"));
}

void test_entity_dictionary::insertOneRemoveFindName()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId = 0;
    EntityComponentMap* map = new EntityComponentMap(entityId, dummyHash);
    dict.insert(entityId, map);

    QVERIFY(dict.setEntityName(entityId, "foo"));
    dict.remove(entityId);
    EntityComponentMap* foundMap = dict.findByName("foo");
    QCOMPARE(foundMap, nullptr);
}

void test_entity_dictionary::entryList()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId0 = 0;
    EntityComponentMap* map0 = new EntityComponentMap(entityId0, dummyHash);
    QVERIFY(dict.insert(entityId0, map0));
    const int entityId1 = 1;
    EntityComponentMap* map1 = new EntityComponentMap(entityId1, dummyHash);
    QVERIFY(dict.insert(entityId1, map1));

    QList<int> list = dict.getEntityList();
    QCOMPARE(list.count(), 2);
    QVERIFY(list.contains(entityId0));
    QVERIFY(list.contains(entityId1));
}

void test_entity_dictionary::nameFromId()
{
    EntityDictionary dict;
    QVariantHash dummyHash;
    const int entityId0 = 0;
    EntityComponentMap* map0 = new EntityComponentMap(entityId0, dummyHash);
    QVERIFY(dict.insert(entityId0, map0));
    const int entityId1 = 1;
    EntityComponentMap* map1 = new EntityComponentMap(entityId1, dummyHash);
    QVERIFY(dict.insert(entityId1, map1));

    QVERIFY(dict.setEntityName(entityId0, "foo"));
    QVERIFY(dict.setEntityName(entityId1, "bar"));

    QCOMPARE(dict.nameFromId(entityId0), "foo");
    QCOMPARE(dict.nameFromId(entityId1), "bar");
    QCOMPARE(dict.nameFromId(42), "");
}

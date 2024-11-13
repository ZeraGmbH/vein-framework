#ifndef TEST_ENTITY_DICTIONARY_H
#define TEST_ENTITY_DICTIONARY_H

#include <QObject>

class test_entity_dictionary : public QObject
{
    Q_OBJECT
private slots:
    void findsOnEmpty();
    void insertOneAndFindById();
    void insertTwoSameId();
    void insertTwoAndFind();
    void insertOneAndDelete();

    void insertOneSetName();
    void insertOneSetEmptyName();
    void insertOneSetSameNameTwice();
    void insertOneSetNameInvalidId();
    void insertOneSetNameFind();
    void insertOneSetTwoNames();
    void insertTwoSetTwoNames();
    void insertTwoSetTwoNamesNotUnique();
    void insertTwoSetFourNames();
    void insertOneRemoveFindName();

    void entryList();
    void nameFromId();
};

#endif // TEST_ENTITY_DICTIONARY_H

#ifndef TEST_CONTAINER_SAFE_DELETE_WHILE_LOOP_H
#define TEST_CONTAINER_SAFE_DELETE_WHILE_LOOP_H

#include <QObject>

class test_container_safe_delete_while_loop : public QObject
{
    Q_OBJECT
private slots:
    void emptyReturnsDefaultValueUnordered();
    void emptyReturnsDefaultValueOrdered();

    void removeOnEmptyUnordered();
    void removeOnEmptyOrdered();

    void addAndIterateUnordered();
    void addAndIterateOrdered();

    void addRemoveAndIterateUnordered();
    void addRemoveAndIterateOrdered();

    void deleteFirstOnFirstIterationUnordered();
    void deleteFirstOnFirstIterationOrdered();

    void deleteSecondOnFirstIterationUnordered();
    void deleteSecondOnFirstIterationOrdered();

    void deleteAllOnFirstIterationUnordered();
    void deleteAllOnFirstIterationOrdered();
};

#endif // TEST_CONTAINER_SAFE_DELETE_WHILE_LOOP_H

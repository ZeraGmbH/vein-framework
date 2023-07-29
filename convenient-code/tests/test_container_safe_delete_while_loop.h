#ifndef TEST_CONTAINER_SAFE_DELETE_WHILE_LOOP_H
#define TEST_CONTAINER_SAFE_DELETE_WHILE_LOOP_H

#include <QObject>

class test_container_safe_delete_while_loop : public QObject
{
    Q_OBJECT
private slots:
    void emptyReturnsDefaultValue();
    void removeOnEmpty();
    void addAndIterate();
    void addRemoveAndIterate();
    void deleteAllOnFirstIteration();
};

#endif // TEST_CONTAINER_SAFE_DELETE_WHILE_LOOP_H

#ifndef TEST_MUTABLE_COMMAND_EVENT_H
#define TEST_MUTABLE_COMMAND_EVENT_H

#include <QObject>

class test_storage : public QObject
{
    Q_OBJECT
private slots:
    void systemEntityOnly();
    void addEntity();
    void addEntityAndComponent();
    void addRemoveEntity();
    void addRemoveComponent();
    void setComponent();
    // fetch component is not testable here but tested by other tests
};

#endif // TEST_MUTABLE_COMMAND_EVENT_H

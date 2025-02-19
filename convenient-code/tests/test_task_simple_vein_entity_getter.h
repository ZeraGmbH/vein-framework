#ifndef TEST_TASK_SIMPLE_VEIN_ENTITY_GETTER_H
#define TEST_TASK_SIMPLE_VEIN_ENTITY_GETTER_H

#include <QObject>

class test_task_simple_vein_entity_getter : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void getAllComponentsValid();
    void getInvalidEntity();
    void fireSameTaskTwiceSameTime();
    void fireSameTaskTwiceAfterAnother();
    void getEntityWithInvalidComponent();
};

#endif // TEST_TASK_SIMPLE_VEIN_ENTITY_GETTER_H

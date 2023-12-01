#ifndef TEST_TASK_SIMPLE_VEIN_GETTER_H
#define TEST_TASK_SIMPLE_VEIN_GETTER_H

#include <QObject>

class test_task_simple_vein_getter : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void getValid();
    void getInvalid();
    void getTimeout();
};

#endif // TEST_TASK_SIMPLE_VEIN_GETTER_H

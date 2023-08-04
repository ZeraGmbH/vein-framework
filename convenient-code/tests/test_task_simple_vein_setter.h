#ifndef TEST_TASK_SIMPLE_VEIN_SETTER_H
#define TEST_TASK_SIMPLE_VEIN_SETTER_H

#include <QObject>

class test_task_simple_vein_setter : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void setValid();
    void setInvalid();
    void setTimeout();

private:
    void feedEventLoop();
};

#endif // TEST_TASK_SIMPLE_VEIN_SETTER_H

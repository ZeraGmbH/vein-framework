#ifndef TEST_COMPONENT_CHANGE_NOTIFIER_H
#define TEST_COMPONENT_CHANGE_NOTIFIER_H

#include <QObject>

class test_component_change_notifier : public QObject
{
    Q_OBJECT
private slots:
    void ignoreOtherCommandsThanSet();
    void notifySet();
    void inClientServerStack();
private:
    static void feedEventLoop();
};

#endif // TEST_COMPONENT_CHANGE_NOTIFIER_H

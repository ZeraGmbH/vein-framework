#ifndef TEST_VFSIMPLECHANGENOTIFIER_H
#define TEST_VFSIMPLECHANGENOTIFIER_H

#include <QObject>

class test_vfsimplechangenotifier : public QObject
{
    Q_OBJECT
private slots:
    void ignoreOtherCommandsThanSet();
    void notifySet();
    void inClientServerStack();
private:
    static void feedEventLoop();
};

#endif // TEST_VFSIMPLECHANGENOTIFIER_H

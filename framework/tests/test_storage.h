#ifndef TEST_MUTABLE_COMMAND_EVENT_H
#define TEST_MUTABLE_COMMAND_EVENT_H

#include <QObject>

class test_storage : public QObject
{
    Q_OBJECT
private slots:
    void serverSide();
    void clientSide();
};

#endif // TEST_MUTABLE_COMMAND_EVENT_H

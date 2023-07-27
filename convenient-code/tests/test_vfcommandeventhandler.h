#ifndef TEST_VFCOMMANDEVENTHANDLER_H
#define TEST_VFCOMMANDEVENTHANDLER_H

#include <QObject>

class test_vfcommandeventhandler : public QObject
{
    Q_OBJECT
private slots:
    void notificationFilter();
    void transactionFilter();
    void multipleEntities();
    void multipleItems();
    void unknownEventType();
};

#endif // TEST_VFCOMMANDEVENTHANDLER_H

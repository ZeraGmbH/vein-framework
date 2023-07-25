#ifndef TEST_VFSIMPLEGETTER_H
#define TEST_VFSIMPLEGETTER_H

#include "vfcommandeventhandlersystem.h"
#include <QObject>

class test_vfsimplegetter : public QObject
{
    Q_OBJECT
private slots:
    void checkErrorSignalFromUnsubscribedEntityInvalidComponent();
    void checkGetFromUnsubscribedEntityValidComponent();
    void noGetFromUnsubscribedEntityValidComponentNetwork();
    void okGetFromSubscribedEntityValidComponentNetwork();
private:
    void subsribeSystemEntity(VfCommandEventHandlerSystem *cmdEventHandlerSystem);
    void feedEventLoop();
};

#endif // TEST_VFSIMPLEGETTER_H

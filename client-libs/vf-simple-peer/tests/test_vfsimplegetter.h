#ifndef TEST_VFSIMPLEGETTER_H
#define TEST_VFSIMPLEGETTER_H

#include <QObject>

class test_vfsimplegetter : public QObject
{
    Q_OBJECT
private slots:
    void errorSignalFromUnsubscribedEntityInvalidComponentNoNet();
    void getFromUnsubscribedEntityValidComponentNoNet();
    void noGetFromUnsubscribedEntityValidComponentNet();
    void getFromSubscribedEntityValidComponentNet();
    void getFromSubscribedEntityInvalidComponentNet();
    void getTwoDifferentComponent();
private:
    void feedEventLoop();
};

#endif // TEST_VFSIMPLEGETTER_H

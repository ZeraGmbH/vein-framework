#ifndef TEST_VFSIMPLEGETTER_H
#define TEST_VFSIMPLEGETTER_H

#include <QObject>

class test_vfsimplegetter : public QObject
{
    Q_OBJECT
private slots:
    void checkErrorSignalFromUnsubscribedEntityInvalidComponent();
    // storage responses without subscription
    //void checkErrorSignalFromUnsubscribedEntityValidComponent();
    void checkOkSignalFromSubscribedEntityValidComponent();



    void getValidComponent();
private:
    void feedEventLoop();
};

#endif // TEST_VFSIMPLEGETTER_H

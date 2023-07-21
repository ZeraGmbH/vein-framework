#ifndef TEST_VFSENTITYSUBSCRIBED_H
#define TEST_VFSENTITYSUBSCRIBED_H

#include <QObject>

class test_vfsentitysubscribed : public QObject
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

#endif // TEST_VFSENTITYSUBSCRIBED_H

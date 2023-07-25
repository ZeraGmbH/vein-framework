#ifndef TEST_VFSIMPLEGETTER_H
#define TEST_VFSIMPLEGETTER_H

#include <QObject>

class test_vfsimplegetter : public QObject
{
    Q_OBJECT
private slots:
    void checkErrorSignalFromUnsubscribedEntityInvalidComponent();
    void checkGetFromUnsubscribedEntityValidComponent();
    void checkGetFromUnsubscribedEntityValidComponentNetwork();
private:
    void feedEventLoop();
};

#endif // TEST_VFSIMPLEGETTER_H

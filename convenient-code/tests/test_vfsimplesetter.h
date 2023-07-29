#ifndef TEST_VFSIMPLESETTER_H
#define TEST_VFSIMPLESETTER_H

#include <QObject>

class test_vfsimplesetter : public QObject
{
    Q_OBJECT
private slots:
    void setInvalidIsEvil();
    void setEqualEmitsOk();
    void setToInvalidEntity();
    void setvalidEntityNet();
private:
    void feedEventLoop();
};

#endif // TEST_VFSIMPLESETTER_H

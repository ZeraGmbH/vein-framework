#ifndef TEST_ATOMIC_CLIENT_COMPONENT_SETTER_H
#define TEST_ATOMIC_CLIENT_COMPONENT_SETTER_H

#include <QObject>

class test_client_component_setter : public QObject
{
    Q_OBJECT
private slots:
    void setInvalidIsEvil();
    void setEqualEmitsOk();
    void setToInvalidEntity();
    void setvalidEntityNet();
};

#endif // TEST_ATOMIC_CLIENT_COMPONENT_SETTER_H

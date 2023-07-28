#ifndef TEST_COMPONENTITEM_H
#define TEST_COMPONENTITEM_H

#include <QObject>

class test_componentitem : public QObject
{
    Q_OBJECT
private slots:
    void ignoreOtherComponent();
    void acceptSameComponent();
};

#endif // TEST_COMPONENTITEM_H

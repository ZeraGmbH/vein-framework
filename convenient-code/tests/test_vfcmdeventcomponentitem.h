#ifndef TEST_VFCMDEVENTCOMPONENTITEM_H
#define TEST_VFCMDEVENTCOMPONENTITEM_H

#include <QObject>

class test_vfcmdeventcomponentitem : public QObject
{
    Q_OBJECT
private slots:
    void ignoreOtherComponent();
    void acceptSameComponent();
};

#endif // TEST_VFCMDEVENTCOMPONENTITEM_H

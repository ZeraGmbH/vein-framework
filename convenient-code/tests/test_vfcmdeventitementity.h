#ifndef TEST_VFCMDEVENTITEMENTITY_H
#define TEST_VFCMDEVENTITEMENTITY_H

#include <QObject>

class test_vfcmdeventitementity : public QObject
{
    Q_OBJECT
private slots:
    void ignoreOtherComponentSameEntity();
    void acceptSameComponentSameEntity();
    void sameComponentNamesOnDifferentEntities();
};

#endif // TEST_VFCMDEVENTITEMENTITY_H

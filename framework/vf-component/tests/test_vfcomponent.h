#ifndef TEST_VFCOMPONENT_H
#define TEST_VFCOMPONENT_H

#include "vftesttemplate.h"

class test_vfcomponent : public QObject, public VfTestTemplate
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void listenerEmpty();
    void detectDoubleEntityComponents();
    void oneChangeOneEntity();
    void twoChangesOneEntity();
    void twoIdenticalChangesOneEntity();

    void oneChangeTwoEntities();
    void twoChangeTwoEntities();
    void twoChangeTwoEntitiesDiffCompName();
    void twoChangeTwoEntitiesCheckContents();
};

#endif // TEST_VFCOMPONENT_H

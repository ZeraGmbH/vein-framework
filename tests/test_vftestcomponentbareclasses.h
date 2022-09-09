#ifndef TEST_VFTESTCOMPONENTBARECLASSES_H
#define TEST_VFTESTCOMPONENTBARECLASSES_H

#include "vftesttemplate.h"

class test_vftestcomponentbareclasses : public QObject, public VfTestTemplate
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
    void ignoreUnlistenedComponent();
    void ignoreUnlistenedEntity();

    void oneChangeTwoEntities();
    void twoChangeTwoEntities();
    void twoChangeTwoEntitiesDiffCompName();
    void twoChangeTwoEntitiesCheckContents();
};

#endif // TEST_VFTESTCOMPONENTBARECLASSES_H

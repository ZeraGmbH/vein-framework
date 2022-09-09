#ifndef TEST_VFTESTCOMPONENTBARECLASSES_H
#define TEST_VFTESTCOMPONENTBARECLASSES_H

#include "vftestentitycomponentcreator.h"
#include "vftestcomponentchangelistener.h"
#include "vftestcomponentdata.h"
#include <ve_eventhandler.h>

class test_vftestcomponentbareclasses : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void listenerEmpty();
    void detectDoubleEntityComponents();
    void oneChange();
    void twoChange();
    void twoChangeSame();
    void ignoreUnlistened();

private:
    static constexpr int entityId = 1;
    VeinEvent::EventHandler* m_vfEventHandler;
    VfTestEntityComponentCreator* m_vfEntity;
    VfTestComponentData* m_vfComponentData;
    VfTestComponentChangeListener* m_vfListener;
};

#endif // TEST_VFTESTCOMPONENTBARECLASSES_H

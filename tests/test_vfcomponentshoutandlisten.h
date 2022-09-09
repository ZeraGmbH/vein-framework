#ifndef TEST_VFCOMPONENTSHOUTANDLISTEN_H
#define TEST_VFCOMPONENTSHOUTANDLISTEN_H

#include "vftestentitycomponentcreator.h"
#include "vftestcomponentchangelistener.h"
#include "vftestcomponentdata.h"
#include <ve_eventhandler.h>
#include <vs_veinhash.h>

class test_VfComponentShoutAndListen : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void listenerEmpty();
    void shoutAndReceiveOne();

private:
    static constexpr int entityId = 1;
    VeinEvent::EventHandler* m_vfEventHandler;
    VeinStorage::VeinHash *m_vfStorageHash;
    VfTestEntityComponentCreator* m_vfEntity;
    VfTestComponentData* m_vfComponentData;
    VfTestComponentChangeListener* m_vfListener;

};

#endif // TEST_VFCOMPONENTSHOUTANDLISTEN_H

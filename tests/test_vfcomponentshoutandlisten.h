#ifndef TEST_VFCOMPONENTSHOUTANDLISTEN_H
#define TEST_VFCOMPONENTSHOUTANDLISTEN_H

#include "vftestentity.h"
#include "vftestcomponentlistener.h"
#include "vftestcomponentshouter.h"
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
    VfTestEntity* m_vfEntity;
    VfTestComponentShouter* m_vfShouter;
    VfTestComponentListener* m_vfListener;

};

#endif // TEST_VFCOMPONENTSHOUTANDLISTEN_H

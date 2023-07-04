#include "veintestserver.h"

VeinTestServer::VeinTestServer(VeinEvent::EventHandler *eventHandler) :
    m_vfEventHandler(eventHandler)
{
    m_mmController.setStorage(&m_storageSystem);

    // do not reorder??
    m_vfEventHandler->addSubsystem(&m_mmController);
    m_vfEventHandler->addSubsystem(&m_introspectionSystem);
    m_vfEventHandler->addSubsystem(&m_storageSystem);

    m_mmController.initOnce();
}

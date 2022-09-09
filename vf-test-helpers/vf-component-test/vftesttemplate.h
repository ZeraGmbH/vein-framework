#ifndef VFTESTTEMPLATE_H
#define VFTESTTEMPLATE_H

#include "vftestentitycomponentcreator.h"
#include "vftestcomponentchangelistener.h"
#include "vftestcomponentdata.h"
#include <ve_eventhandler.h>

class VfTestTemplate
{
protected:
    void addSecondEntity(QString compName);
    void _init();
    void _cleanup();

    static constexpr int entityId = 1;
    const char* componentName = "fooName";
    VeinEvent::EventHandler* m_vfEventHandler;
    VfTestEntityComponentCreator* m_vfEntity;
    VfTestComponentData* m_vfComponentData;
    VfTestComponentChangeListener* m_vfListener;
};

#endif // VFTESTTEMPLATE_H

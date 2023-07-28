#include "vfsimplechangenotifier.h"
#include <vcmp_componentdata.h>


std::shared_ptr<VfSimpleChangeNotifier> VfSimpleChangeNotifier::create(int entityId, QString componentName)
{
    return std::make_shared<VfSimpleChangeNotifier>(entityId, componentName);
}

VfSimpleChangeNotifier::VfSimpleChangeNotifier(int entityId, QString componentName) :
    VfCommandEventHandlerComponentItem(entityId, componentName)
{
}

const QVariant &VfSimpleChangeNotifier::getValue() const
{
    return m_componentValue;
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfSimpleChangeNotifier::processComponentEventData(const ComponentData *cData)
{
    if(cData->eventCommand() == ComponentData::Command::CCMD_SET) {
        m_componentValue = cData->newValue();
        emit sigValueChanged();
    }
}

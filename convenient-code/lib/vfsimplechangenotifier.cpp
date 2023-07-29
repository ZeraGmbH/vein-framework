#include "vfsimplechangenotifier.h"
#include <vcmp_componentdata.h>


std::shared_ptr<VfSimpleChangeNotifier> VfSimpleChangeNotifier::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfSimpleChangeNotifier>(componentName, entityItem);
}

VfSimpleChangeNotifier::VfSimpleChangeNotifier(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent(componentName, entityItem)
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

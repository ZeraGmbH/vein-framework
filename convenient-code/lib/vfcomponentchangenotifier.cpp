#include "vfcomponentchangenotifier.h"
#include <vcmp_componentdata.h>


std::shared_ptr<VfComponentChangeNotifier> VfComponentChangeNotifier::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfComponentChangeNotifier>(componentName, entityItem);
}

VfComponentChangeNotifier::VfComponentChangeNotifier(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent(componentName, entityItem)
{
}

const QVariant &VfComponentChangeNotifier::getValue() const
{
    return m_componentValue;
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfComponentChangeNotifier::processComponentEventData(const ComponentData *cData)
{
    if(cData->eventCommand() == ComponentData::Command::CCMD_SET) {
        m_componentValue = cData->newValue();
        emit sigValueChanged();
    }
}
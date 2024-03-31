#include "vcmp_errordatasender.h"
#include "vcmp_errordata.h"
#include "ve_commandevent.h"
#include "ve_eventdata.h"

using namespace VeinEvent;
using namespace VeinComponent;

void ErrorDataSender::errorOut(QString errorMessage, QEvent *event, VeinEvent::EventSystem *eventSystem)
{
    qWarning() << errorMessage;

    CommandEvent *cEvent = static_cast<CommandEvent*>(event);
    EventData *data = cEvent->eventData();
    ErrorData *errData = new ErrorData();
    errData->setEntityId(data->entityId());
    errData->setOriginalData(data);
    errData->setEventOrigin(EventData::EventOrigin::EO_LOCAL);
    errData->setEventTarget(data->eventTarget());
    errData->setErrorDescription(errorMessage);

    event->accept();

    CommandEvent *errEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, errData);
    errEvent->setPeerId(cEvent->peerId());

    emit eventSystem->sigSendEvent(errEvent);
}

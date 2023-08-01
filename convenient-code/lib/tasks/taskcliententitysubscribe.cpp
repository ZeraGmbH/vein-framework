#include "taskcliententitysubscribe.h"
#include <taskdecoratortimeout.h>

TaskTemplatePtr TaskClientEntitySubscribe::create(int entityId, VfCommandEventHandlerSystemPtr commandEventHandler,
                                                  std::shared_ptr<QStringList> componentNames, int timeout, std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskClientEntitySubscribe>(entityId, commandEventHandler, componentNames),
                                             additionalErrorHandler);
}

TaskClientEntitySubscribe::TaskClientEntitySubscribe(int entityId, VfCommandEventHandlerSystemPtr commandEventHandler,
                                                     std::shared_ptr<QStringList> componentNames) :
    m_commandEventHandler(commandEventHandler),
    m_componentNames(componentNames)
{
    m_subscriber = VfAtomicClientEntitySubscriber::create(entityId);
    m_commandEventHandler->addItem(m_subscriber);
}

TaskClientEntitySubscribe::~TaskClientEntitySubscribe()
{
    m_commandEventHandler->removeItem(m_subscriber);
}

void TaskClientEntitySubscribe::start()
{
    connect(m_subscriber.get(), &VfAtomicClientEntitySubscriber::sigSubscribed, this, [&](bool ok, int entityId) {
        Q_UNUSED(entityId)
        *m_componentNames = m_subscriber->getComponentNames();
        finishTask(ok);
    });
    m_subscriber->sendSubscription();
}

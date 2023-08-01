#include "taskclientcomponentfetcher.h"
#include <taskdecoratortimeout.h>

TaskTemplatePtr TaskClientComponentFetcher::create(QString componentName, VfCmdEventItemEntityPtr entityItem,
                                                   std::shared_ptr<QVariant> value, int timeout, std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskClientComponentFetcher>(componentName, entityItem, value),
                                             additionalErrorHandler);
}

TaskClientComponentFetcher::TaskClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem,
                                                       std::shared_ptr<QVariant> value) :
    m_value(value),
    m_entityItem(entityItem)
{
    m_fetcher = VfAtomicClientComponentFetcher::create(componentName, entityItem);
    m_entityItem->addItem(m_fetcher);
}

TaskClientComponentFetcher::~TaskClientComponentFetcher()
{
    m_entityItem->removeItem(m_fetcher);
}

void TaskClientComponentFetcher::start()
{
    connect(m_fetcher.get(), &VfAtomicClientComponentFetcher::sigGetFinish, this, [&](bool ok, QVariant value) {
        *m_value = value;
        finishTask(ok);
    });
    m_fetcher->startGetComponent();
}

#include "task_client_component_fetcher.h"
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
    m_fetcher = VfClientComponentFetcher::create(componentName, entityItem);
    m_entityItem->addItem(m_fetcher);
}

TaskClientComponentFetcher::~TaskClientComponentFetcher()
{
    m_entityItem->removeItem(m_fetcher);
}

void TaskClientComponentFetcher::start()
{
    connect(m_fetcher.get(), &VfClientComponentFetcher::sigGetFinish, this, [&](bool ok, QVariant value) {
        *m_value = value;
        finishTask(ok);
    });
    m_fetcher->startGetComponent();
}

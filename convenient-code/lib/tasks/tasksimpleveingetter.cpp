#include "tasksimpleveingetter.h"
#include "task_client_entity_subscribe.h"
#include "task_client_component_fetcher.h"

std::unique_ptr<TaskSimpleVeinGetter> TaskSimpleVeinGetter::create(int entityId, QString componentName,
                                                                   VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout)
{
    return std::make_unique<TaskSimpleVeinGetter>(entityId, componentName, cmdEventHandlerSystem, timeout);
}

TaskSimpleVeinGetter::TaskSimpleVeinGetter(int entityId, QString componentName,
                                           VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout):
    m_value(std::make_shared<QVariant>()),
    m_entityItem(VfEntityComponentEventItem::create(entityId)),
    m_cmdEventHandlerSystem(cmdEventHandlerSystem)
{
    std::shared_ptr<QStringList> componentList = std::make_shared<QStringList>();
    m_cmdEventHandlerSystem->addItem(m_entityItem);
    m_task.addSub(TaskClientEntitySubscribe::create(entityId, cmdEventHandlerSystem, componentList, timeout));
    m_task.addSub(TaskClientComponentFetcher::create(componentName, m_entityItem, m_value, timeout));
    connect(&m_task, &TaskTemplate::sigFinish, this, &TaskTemplate::sigFinish);
}

TaskSimpleVeinGetter::~TaskSimpleVeinGetter()
{
    m_cmdEventHandlerSystem->removeItem(m_entityItem);
}

QVariant TaskSimpleVeinGetter::getValue()
{
    return *m_value;
}

void TaskSimpleVeinGetter::start()
{
    m_task.start();
}


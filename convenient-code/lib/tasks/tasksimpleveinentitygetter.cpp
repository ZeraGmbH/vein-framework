#include "tasksimpleveinentitygetter.h"
#include "task_client_component_fetcher.h"
#include "taskcontainerparallel.h"

std::unique_ptr<TaskSimpleVeinEntityGetter> TaskSimpleVeinEntityGetter::create(int entityId, VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout)
{
    return std::make_unique<TaskSimpleVeinEntityGetter>(entityId, cmdEventHandlerSystem, timeout);
}

TaskSimpleVeinEntityGetter::TaskSimpleVeinEntityGetter(int entityId, VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout) :
    m_entityItem(VfEntityComponentEventItem::create(entityId)),
    m_cmdEventHandlerSystem(cmdEventHandlerSystem),
    m_timeout(timeout)
{
    m_componentList = std::make_shared<QStringList>();
    m_cmdEventHandlerSystem->addItem(m_entityItem);
    m_fetcherTasks = TaskContainerParallel::create();
    m_task = TaskClientEntitySubscribe::create(entityId, cmdEventHandlerSystem, m_componentList, timeout);
    connect(m_task.get(), &TaskTemplate::sigFinish, this, &TaskSimpleVeinEntityGetter::startComponentFetcher);
}

TaskSimpleVeinEntityGetter::~TaskSimpleVeinEntityGetter()
{
    m_cmdEventHandlerSystem->removeItem(m_entityItem);
}

void TaskSimpleVeinEntityGetter::start()
{
    m_task->start();
}

QList<std::shared_ptr<QVariant> > TaskSimpleVeinEntityGetter::getValues()
{
    return m_values;
}

void TaskSimpleVeinEntityGetter::startComponentFetcher(bool ok, int taskId)
{
    if(ok)
    {
        for (const auto &item : *m_componentList)
        {
            std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
            m_values.append(value);
            TaskTemplatePtr singleFetcher = TaskClientComponentFetcher::create(item, m_entityItem, value, m_timeout);
            m_fetcherTasks->addSub(std::move(singleFetcher));
        }
        connect(m_fetcherTasks.get(), &TaskTemplate::sigFinish, this, [this](bool ok, int taskId){
            finishTask(ok);
        });
        m_fetcherTasks->start();
    }
    else
    {
        finishTask(false);
    }
}

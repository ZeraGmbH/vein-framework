#ifndef TASKSIMPLEVEINENTITYGETTER_H
#define TASKSIMPLEVEINENTITYGETTER_H

#include "taskcontainerinterface.h"
#include "vf_cmd_event_handler_system.h"
#include "vf_entity_component_event_item.h"
#include "task_client_entity_subscribe.h"
#include <tasktemplate.h>


class TaskSimpleVeinEntityGetter : public TaskTemplate
{
    Q_OBJECT
public:
    static std::unique_ptr<TaskSimpleVeinEntityGetter> create(int entityId, VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout = 1000);
    TaskSimpleVeinEntityGetter(int entityId, VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout);
    virtual ~TaskSimpleVeinEntityGetter();

    void start() override;
    QList<std::shared_ptr<QVariant>> getValues();

private:
    int m_timeout;
    TaskTemplatePtr m_task;
    std::shared_ptr<QStringList> m_componentList;
    VfCmdEventItemEntityPtr m_entityItem;
    VfCmdEventHandlerSystemPtr m_cmdEventHandlerSystem;
    TaskContainerInterfacePtr m_fetcherTasks;
    QList<std::shared_ptr<QVariant>> m_values;

private slots:
    void startComponentFetcher(bool ok, int taskId);
};

typedef std::unique_ptr<TaskSimpleVeinEntityGetter> TaskSimpleVeinEntityGetterPtr;

#endif // TASKSIMPLEVEINENTITYGETTER_H

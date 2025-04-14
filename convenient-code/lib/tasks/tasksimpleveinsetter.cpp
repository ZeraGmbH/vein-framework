#include "tasksimpleveinsetter.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "task_client_entity_subscribe.h"
#include "task_client_component_setter.h"
#include "task_client_component_fetcher.h"

std::unique_ptr<TaskSimpleVeinSetter> TaskSimpleVeinSetter::create(int entityId, QString componentName, QVariant newValue,
                                                                   VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout)
{
    return std::make_unique<TaskSimpleVeinSetter>(entityId, componentName, newValue, cmdEventHandlerSystem, timeout);
}

TaskSimpleVeinSetter::TaskSimpleVeinSetter(int entityId, QString componentName, QVariant newValue,
                                           VfCmdEventHandlerSystemPtr cmdEventHandlerSystem, int timeout):
    m_oldValue(std::make_shared<QVariant>()),
    m_entityItem(VfEntityComponentEventItem::create(entityId)),
    m_cmdEventHandlerSystem(cmdEventHandlerSystem)
{
    std::shared_ptr<QStringList> componentList = std::make_shared<QStringList>();
    m_cmdEventHandlerSystem->addItem(m_entityItem);
    m_taskGet.addSub(TaskClientEntitySubscribe::create(entityId, cmdEventHandlerSystem, componentList, timeout, [](){
        qWarning("Subscriber Task failed");
    }));
    m_taskGet.addSub(TaskClientComponentFetcher::create(componentName, m_entityItem, m_oldValue, timeout, [](){
        qWarning("Getter Task failed");
    }));
    connect(&m_taskGet, &TaskTemplate::sigFinish, this, [this, componentName, newValue, timeout](bool ok, int taskId) {
        if(ok) {
            QVariant valueToBeSent = newValue;
            if (static_cast<QMetaType::Type>(m_oldValue->type()) == QMetaType::QJsonObject) {
                QString data(valueToBeSent.toString());
                QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
                valueToBeSent = doc.object();
            }
            m_taskSet.addSub(TaskClientComponentSetter::create(m_entityItem, componentName, *m_oldValue, valueToBeSent, timeout, []() {
                qWarning("Setter Task failed");
            }));
            connect(&m_taskSet, &TaskTemplate::sigFinish, this, &TaskSimpleVeinSetter::sigFinish);
            m_taskSet.start();
        }
        else
            emit sigFinish(ok, taskId);
    });
}

TaskSimpleVeinSetter::~TaskSimpleVeinSetter()
{
    m_cmdEventHandlerSystem->removeItem(m_entityItem);
}

void TaskSimpleVeinSetter::start()
{
    m_taskGet.start();
}


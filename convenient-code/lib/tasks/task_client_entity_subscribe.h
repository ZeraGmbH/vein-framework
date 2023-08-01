#ifndef TASKCLIENTENTITYSUBSCRIBE_H
#define TASKCLIENTENTITYSUBSCRIBE_H

#include "vf_client_entity_subscriber.h"
#include "vf_cmd_event_handler_system.h"
#include <tasktemplate.h>
#include <QObject>

class TaskClientEntitySubscribe : public TaskTemplate
{
public:
    static TaskTemplatePtr create(int entityId, VfCmdEventHandlerSystemPtr commandEventHandler,
                                  std::shared_ptr<QStringList> componentNames,
                                  int timeout, std::function<void()> additionalErrorHandler = []{});
    TaskClientEntitySubscribe(int entityId, VfCmdEventHandlerSystemPtr commandEventHandler,
                              std::shared_ptr<QStringList> componentNames);
    ~TaskClientEntitySubscribe();
    void start() override;
private:
    VfCmdEventHandlerSystemPtr m_commandEventHandler;
    VfClientEntitySubscriberPtr m_subscriber;
    std::shared_ptr<QStringList> m_componentNames;
};

#endif // TASKCLIENTENTITYSUBSCRIBE_H

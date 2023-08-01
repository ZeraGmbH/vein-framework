#ifndef TASKCLIENTENTITYSUBSCRIBE_H
#define TASKCLIENTENTITYSUBSCRIBE_H

#include "vfatomiccliententitysubscriber.h"
#include "vfcommandeventhandlersystem.h"
#include <tasktemplate.h>
#include <QObject>

class TaskClientEntitySubscribe : public TaskTemplate
{
public:
    static TaskTemplatePtr create(int entityId, VfCommandEventHandlerSystemPtr commandEventHandler,
                                  std::shared_ptr<QStringList> componentNames,
                                  int timeout, std::function<void()> additionalErrorHandler = []{});
    TaskClientEntitySubscribe(int entityId, VfCommandEventHandlerSystemPtr commandEventHandler,
                              std::shared_ptr<QStringList> componentNames);
    ~TaskClientEntitySubscribe();
    void start() override;
private:
    VfCommandEventHandlerSystemPtr m_commandEventHandler;
    VfAtomicClientEntitySubscriberPtr m_subscriber;
    std::shared_ptr<QStringList> m_componentNames;
};

#endif // TASKCLIENTENTITYSUBSCRIBE_H

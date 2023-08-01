#ifndef TASKCLIENTCOMPONENTFETCHER_H
#define TASKCLIENTCOMPONENTFETCHER_H

#include "vfatomicclientcomponentfetcher.h"
#include "vfcommandeventhandlersystem.h"
#include <tasktemplate.h>
#include <QObject>

class TaskClientComponentFetcher : public TaskTemplate
{
public:
    static TaskTemplatePtr create(QString componentName, VfCmdEventItemEntityPtr entityItem,
                                  std::shared_ptr<QVariant> value,
                                  int timeout, std::function<void()> additionalErrorHandler = []{});
    TaskClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem,
                               std::shared_ptr<QVariant> value);
    ~TaskClientComponentFetcher();
    void start() override;
private:
    std::shared_ptr<QVariant> m_value;
    VfCmdEventItemEntityPtr m_entityItem;
    VfAtomicClientComponentFetcherPtr m_fetcher;
};

#endif // TASKCLIENTCOMPONENTFETCHER_H

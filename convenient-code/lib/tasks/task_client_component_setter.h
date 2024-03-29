#ifndef TASKCLIENTCOMPONENTSETTER_H
#define TASKCLIENTCOMPONENTSETTER_H

#include "vf_client_component_setter.h"
#include <tasktemplate.h>

class TaskClientComponentSetter : public TaskTemplate
{
    Q_OBJECT
public:
    static TaskTemplatePtr create(VfCmdEventItemEntityPtr entityItem,
                                  QString componentName, QVariant oldValue, QVariant newValue,
                                  int timeout, std::function<void()> additionalErrorHandler = []{});
    TaskClientComponentSetter(VfCmdEventItemEntityPtr entityItem,
                              QString componentName, QVariant oldValue, QVariant newValue);
    ~TaskClientComponentSetter();
    void start() override;
private:
    VfCmdEventItemEntityPtr m_entityItem;
    VfClientComponentSetterPtr m_setter;
    QVariant m_oldValue;
    QVariant m_newValue;
};

#endif // TASKCLIENTCOMPONENTSETTER_H

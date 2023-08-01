#include "taskclientcomponentsetter.h"
#include <taskdecoratortimeout.h>

TaskTemplatePtr TaskClientComponentSetter::create(VfCmdEventItemEntityPtr entityItem,
                                                  QString componentName, QVariant oldValue, QVariant newValue,
                                                  int timeout, std::function<void ()> additionalErrorHandler)
{
    return TaskDecoratorTimeout::wrapTimeout(timeout,
                                             std::make_unique<TaskClientComponentSetter>(entityItem, componentName, oldValue, newValue),
                                             additionalErrorHandler);
}

TaskClientComponentSetter::TaskClientComponentSetter(VfCmdEventItemEntityPtr entityItem,
                                                     QString componentName, QVariant oldValue, QVariant newValue) :
    m_entityItem(entityItem),
    m_oldValue(oldValue),
    m_newValue(newValue)
{
    m_setter = VfAtomicClientComponentSetter::create(componentName, m_entityItem);
    m_entityItem->addItem(m_setter);
}

TaskClientComponentSetter::~TaskClientComponentSetter()
{
    m_entityItem->removeItem(m_setter);
}

void TaskClientComponentSetter::start()
{
    connect(m_setter.get(), &VfAtomicClientComponentSetter::sigSetFinish, this, [&](bool ok) {
        finishTask(ok);
    });
    m_setter->startSetComponent(m_oldValue, m_newValue);
}


#ifndef VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H
#define VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H

#include <vfcmdeventitemcomponent.h>
#include <QObject>

class VfCommandEventHandlerComponentItemTest : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfCommandEventHandlerComponentItemTest> create(QString componentName, VfCmdEventItemEntityPtr entityItem);
    VfCommandEventHandlerComponentItemTest(QString componentName, VfCmdEventItemEntityPtr entityItem);

    void processComponentEventData(const VeinComponent::ComponentData *componentData) override;
    void processErrorComonentEventData(const VeinComponent::ComponentData *originalComponentData) override;
signals:
    void sigCommandEvenProcessed();
private:
};

typedef std::shared_ptr<VfCommandEventHandlerComponentItemTest> VfCommandEventHandlerComponentItemTestPtr;

#endif // VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H

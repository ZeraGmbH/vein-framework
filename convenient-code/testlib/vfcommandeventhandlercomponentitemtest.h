#ifndef VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H
#define VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H

#include <vfcmdeventitemcomponent.h>
#include <QObject>

class VfCommandEventHandlerComponentItemTest : public QObject, public VfCmdEventItemComponent
{
    Q_OBJECT
public:
    static std::shared_ptr<VfCommandEventHandlerComponentItemTest> create(int entityId, QString componentName);
    VfCommandEventHandlerComponentItemTest(int entityId, QString componentName);
signals:
    void sigCommandEvenProcessed();
private:
    void processComponentEventData(const VeinComponent::ComponentData *cData) override;
};

typedef std::shared_ptr<VfCommandEventHandlerComponentItemTest> VfCommandEventHandlerComponentItemTestPtr;

#endif // VFCOMMANDEVENTHANDLERCOMPONENTITEMTEST_H

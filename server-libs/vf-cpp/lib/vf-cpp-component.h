#ifndef VFCPPCOMPONENT_H
#define VFCPPCOMPONENT_H

#include <vcmp_componentdata.h>
#include <vcmp_errordata.h>
#include <ve_eventsystem.h>

namespace VfCpp {

// a vein-only extract of VfModuleComponent
class VfCppComponent: public QObject
{
    Q_OBJECT
public:
    typedef  QSharedPointer< VfCppComponent > Ptr;
    VfCppComponent(int entityId, VeinEvent::EventSystem *eventsystem, QString name, QVariant initval, bool readOnly=false);
    QVariant getValue();
    QString getName();

    void setValue(QVariant value); // spawn vein event
    void setValueFromVein(QVariant value); // just for VfCppEntity...
    void changeComponentReadWriteType(bool readWrite);

signals:
    void sigValueChanged(QVariant);

private:
    void sendNotification(VeinComponent::ComponentData::Command vcmd);
    int m_nEntityId;
    VeinEvent::EventSystem *m_pEventSystem;
    QString m_sName;
    QVariant m_vValue;
    bool m_readOnly;
};
}

#endif // VFCPPCOMPONENT_H

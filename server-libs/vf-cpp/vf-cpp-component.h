#ifndef VEINMODULECOMPONENT_H
#define VEINMODULECOMPONENT_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QUuid>

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
    ~VfCppComponent();
    QVariant getValue();
    QString getName();
    // spawn vein event
    void setValue(QVariant value); // here we have to emit event for notification
    void setError(); // here we have to emit event for error notification
    // just for VfCppEntity...
    void setValueByEvent(QVariant value);
signals:
    void sigValueChanged(QVariant); // we connect here if we want to do something on changed values
    void sigValueQuery(QVariant); // we connect here if we want to read a value before returning data from storage ...perhaps with parameter
protected:
    int m_nEntityId;
    VeinEvent::EventSystem *m_pEventSystem;
    QString m_sName;
    QVariant m_vValue;
    bool m_readOnly;
protected:
    void sendNotification(VeinComponent::ComponentData::Command vcmd);
};
}

#endif // VEINMODULECOMPONENT_H

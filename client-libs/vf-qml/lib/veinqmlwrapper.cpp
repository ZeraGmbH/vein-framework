#include "veinqmlwrapper.h"
#include <QCoreApplication>

#include <qqml.h>

#include <veinqml.h>
#include <entitycomponentmap.h>

namespace VeinApiQml
{

QmlWrapper::QmlWrapper()
{
}

QObject *QmlWrapper::getSingletonInstance(QQmlEngine *t_engine, QJSEngine *t_scriptEngine)
{
    Q_UNUSED(t_engine);
    Q_UNUSED(t_scriptEngine);
    return VeinApiQml::VeinQml::getStaticInstance();
}

void registerTypes()
{
    // @uri Vein
    using namespace VeinApiQml;
    qmlRegisterSingletonType<VeinQml>("VeinEntity", 1, 0, "VeinEntity", QmlWrapper::getSingletonInstance);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    qmlRegisterInterface<EntityComponentMap>("EntityComponentMap");
#else
    qmlRegisterInterface<EntityComponentMap>("EntityComponentMap", 1);
#endif
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)


} // namespace VeinApiQml


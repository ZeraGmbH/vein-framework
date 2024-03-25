#ifndef VH_LOGGING
#define VH_LOGGING

#include <QLoggingCategory>

#ifdef VF_NO_DEBUG
#define vCDebug(X) QNoDebug()
#else
#define vCDebug(X) qCDebug(X)
#endif

// if __PRETTY_FUNCTION__ does not work with your comiler then add some conditions here
#define VF_ASSERT(test, what) Q_ASSERT_X(test, __PRETTY_FUNCTION__, what)
#define QStringC(X) (X).toStdString().c_str()

#if defined(VEIN_COLORS_SUPPORT)
# define VEIN_DEBUGNAME_EVENT "\e[1;34m<Vein.Event>\033[0m"
# define VEIN_DEBUGNAME_COMPONENT "\e[1;34m<Vein.Event>\033[0m"
# define VEIN_DEBUGNAME_LOGGER "\e[0;34m<Vein.Logger>\033[0m"
# define VEIN_DEBUGNAME_NET "\e[1;32m<Vein.Network>\033[0m"
# define VEIN_DEBUGNAME_NET_VERBOSE "\e[0;32m<Vein.Network>\033[0m"
# define VEIN_DEBUGNAME_NET_INTRO "\e[1;35m<Vein.Network.Introspection>\033[0m"
# define VEIN_DEBUGNAME_NET_INTRO_VERBOSE "\e[0;35m<Vein.Network.Introspection>\033[0m"
# define VEIN_DEBUGNAME_SCRIPT "\e[1;37m<Vein.Script>\033[0m"
# define VEIN_DEBUGNAME_QML "\e[1;37m<Vein.Api.QML>\033[0m"
# define VEIN_DEBUGNAME_QML_INTROSPECTION "\e[1;37m<Vein.Api.QML.Introspection>\033[0m"
# define VEIN_DEBUGNAME_QML_VERBOSE "\e[0;37m<Vein.Api.QML>\033[0m"
# define VEIN_DEBUGNAME_NET_TCP "\e[1;33m<Vein.Network.Tcp>\033[0m"
# define VEIN_DEBUGNAME_NET_TCP_VERBOSE "\e[0;33m<Vein.Network.Tcp>\033[0m"
#else
# define VEIN_DEBUGNAME_EVENT "<Vein.Event>"
# define VEIN_DEBUGNAME_COMPONENT "<Vein.Event>"
# define VEIN_DEBUGNAME_LOGGER "<Vein.Logger>"
# define VEIN_DEBUGNAME_NET "<Vein.Network>"
# define VEIN_DEBUGNAME_NET_VERBOSE "<Vein.Network>"
# define VEIN_DEBUGNAME_NET_INTRO "<Vein.Network.Introspection>"
# define VEIN_DEBUGNAME_NET_INTRO_VERBOSE "<Vein.Network.Introspection>"
# define VEIN_DEBUGNAME_SCRIPT "<Vein.Script>"
# define VEIN_DEBUGNAME_QML "<Vein.Api.QML>"
# define VEIN_DEBUGNAME_QML_INTROSPECTION "<Vein.Api.QML.Introspection>"
# define VEIN_DEBUGNAME_QML_VERBOSE "<Vein.Api.QML>"
# define VEIN_DEBUGNAME_NET_TCP "<Vein.Network.Tcp>"
# define VEIN_DEBUGNAME_NET_TCP_VERBOSE "<Vein.Network.Tcp>"
#endif //defined(VEIN_COLORS_SUPPORT)

#endif // VH_LOGGING

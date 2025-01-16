#include "xsessionselector.h"
#include "lxdmconffilemodifier.h"

XSessionSelector::XSessionSelector()
{
    m_xSessions["Desktop"] = "/usr/share/xsessions/xfce.desktop";
    m_xSessions["Fullscreen"] = "/usr/share/xsessions/zenux-gui.desktop";
}

QStringList XSessionSelector::availableSessions()
{
    return m_xSessions.keys();
}

bool XSessionSelector::switchSession(const QString &displayName)
{
    const auto iter = m_xSessions.constFind(displayName);
    if(iter == m_xSessions.constEnd()) {
        qCritical("%s is not a known xsession!", qPrintable(displayName));
        return false;
    }

    if(!LxdmConfFileModifier::modifyConfig(iter.value()))
        return false;

    if(system("systemctl restart lxdm.service") != 0) {
        qCritical("Cannot restart lxdm!");
        return false;
    }
    return true;
}


#ifndef XSESSIONSELECTOR_H
#define XSESSIONSELECTOR_H

#include <QMap>

class XSessionSelector
{
public:
    XSessionSelector();
    QStringList availableSessions();
    bool switchSession(const QString &displayName);
private:
    QMap<QString /*displayName*/, QString /*sessionFileName*/> m_xSessions;
};

#endif // XSESSIONSELECTOR_H

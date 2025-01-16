#ifndef LXDMCONFFILEMODIFIER_H
#define LXDMCONFFILEMODIFIER_H

#include <QString>

class LxdmConfFileModifier
{
public:
    static bool modifyConfig(const QString &sessionFileName, const QString &fileName = "/var/lib/lxdm/lxdm.conf");
};

#endif // LXDMCONFFILEMODIFIER_H

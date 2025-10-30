#ifndef VFMODULEMETAINFOCONTAINER_H
#define VFMODULEMETAINFOCONTAINER_H

#include <QString>
#include <QJsonArray>

namespace SCPI {
enum eSCPIEntryType {
    isComponent,
    isCatalog
};
}

class VfModuleMetaInfoContainer
{
public:
    explicit VfModuleMetaInfoContainer(const QString &model, const QString &cmd,
                                       int cmdTypeMask, // e.g SCPI::isQuery|SCPI::isCmdwP
                                       const QString &veinComponentNameOrRpcSignature,
                                       SCPI::eSCPIEntryType entryType = SCPI::isComponent);
    void appendSCPIInfo(QJsonArray &jsArr);

private:
    const QString m_sSCPIModel;
    const QString m_sSCPICmd;
    const QString m_sSCPICmdType;
    const QString m_veinComponentNameOrRpcSignature;
    const QString m_sRefType;
};

#endif // VFMODULEMETAINFOCONTAINER_H

#include "vfmodulemetainfocontainer.h"

VfModuleMetaInfoContainer::VfModuleMetaInfoContainer(const QString &model,
                                             const QString &cmd,
                                             int cmdTypeMask,
                                             const QString &veinComponentNameOrRpcSignature,
                                             SCPI::eSCPIEntryType entryType) :
    m_sSCPIModel(model),
    m_sSCPICmd(cmd),
    m_sSCPICmdType(QString::number(cmdTypeMask)),
    m_veinComponentNameOrRpcSignature(veinComponentNameOrRpcSignature),
    m_sRefType(entryType==SCPI::isCatalog ? "1" : "0")
{
}

void VfModuleMetaInfoContainer::appendSCPIInfo(QJsonArray &jsArr)
{
    QJsonArray jsonSCPIArr;
    jsonSCPIArr.append(m_sSCPIModel);
    jsonSCPIArr.append(m_sSCPICmd);
    jsonSCPIArr.append(m_sSCPICmdType);
    jsonSCPIArr.append(m_veinComponentNameOrRpcSignature);
    jsonSCPIArr.append(m_sRefType);
    jsArr.append(jsonSCPIArr);
}

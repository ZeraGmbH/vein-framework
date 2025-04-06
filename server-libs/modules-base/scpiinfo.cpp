#include "scpiinfo.h"

cSCPIInfo::cSCPIInfo(QString model, QString cmd, QString cmdtype, QString veinComponentName, QString reftype, QString unit) :
    m_sSCPIModel(model),
    m_sSCPICmd(cmd),
    m_sSCPICmdType(cmdtype),
    m_veinComponentName(veinComponentName),
    m_sRefType(reftype)
{
    Q_UNUSED(unit)
}

cSCPIInfo::cSCPIInfo(QString model,
                     QString cmd,
                     int cmdTypeMask,
                     QString veinComponentName,
                     SCPI::eSCPIEntryType entryType) :
    m_sSCPIModel(model),
    m_sSCPICmd(cmd),
    m_sSCPICmdType(QString::number(cmdTypeMask)),
    m_veinComponentName(veinComponentName),
    m_sRefType(entryType==SCPI::isCatalog ? "1" : "0")
{
}

void cSCPIInfo::appendSCPIInfo(QJsonArray &jsArr)
{
    QJsonArray jsonSCPIArr;
    jsonSCPIArr.append(m_sSCPIModel);
    jsonSCPIArr.append(m_sSCPICmd);
    jsonSCPIArr.append(m_sSCPICmdType);
    jsonSCPIArr.append(m_veinComponentName);
    jsonSCPIArr.append(m_sRefType);

    jsArr.append(jsonSCPIArr);
}

#include "scpiinfo.h"

cSCPIInfo::cSCPIInfo(QString model, QString cmd, QString cmdtype, QString veinComponentName, QString reftype, QString unit) :
    m_sSCPIModel(model), m_sSCPICmd(cmd), m_sSCPICmdType(cmdtype), m_veinComponentName(veinComponentName), m_sRefType(reftype), m_sUnit(unit)
{
}

cSCPIInfo::cSCPIInfo(QString model,
                     QString cmd,
                     int cmdTypeMask,
                     QString veinComponentName,
                     SCPI::eSCPIEntryType entryType,
                     QString unit) :
    m_sSCPIModel(model),
    m_sSCPICmd(cmd),
    m_sSCPICmdType(QString::number(cmdTypeMask)),
    m_veinComponentName(veinComponentName),
    m_sRefType(entryType==SCPI::isCatalog ? "1" : "0"),
    m_sUnit(unit)
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
    jsonSCPIArr.append(m_sUnit);

    jsArr.append(jsonSCPIArr);
}

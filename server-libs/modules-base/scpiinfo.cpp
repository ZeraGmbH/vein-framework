#include "scpiinfo.h"

cSCPIInfo::cSCPIInfo(QString model, QString cmd, QString cmdtype, QString refname, QString reftype, QString unit)
    :m_sSCPIModel(model), m_sSCPICmd(cmd), m_sSCPICmdType(cmdtype), m_sRefName(refname), m_sRefType(reftype), m_sUnit(unit)
{
}

void cSCPIInfo::appendSCPIInfo(QJsonArray &jsArr)
{
    QJsonArray jsonSCPIArr;
    jsonSCPIArr.append(m_sSCPIModel);
    jsonSCPIArr.append(m_sSCPICmd);
    jsonSCPIArr.append(m_sSCPICmdType);
    jsonSCPIArr.append(m_sRefName);
    jsonSCPIArr.append(m_sRefType);
    jsonSCPIArr.append(m_sUnit);

    jsArr.append(jsonSCPIArr);
}

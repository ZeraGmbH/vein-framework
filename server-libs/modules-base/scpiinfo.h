#ifndef SCPIINFO_H
#define SCPIINFO_H

#include <QString>
#include <QJsonArray>

class cSCPIInfo
{
public:
    cSCPIInfo(){}
    cSCPIInfo(QString model, QString cmd, QString cmdtype, QString refname, QString reftype, QString unit);

    void appendSCPIInfo(QJsonArray &jsArr);

private:
    const QString m_sSCPIModel;
    const QString m_sSCPICmd;
    const QString m_sSCPICmdType;
    const QString m_sRefName;
    const QString m_sRefType;
    const QString m_sUnit;
};


#endif // SCPIINFO_H

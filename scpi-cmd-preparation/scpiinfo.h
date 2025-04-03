#ifndef SCPIINFO_H
#define SCPIINFO_H

#include <QString>
#include <QJsonObject>


class cSCPIInfo
{
public:
    cSCPIInfo(){}
    cSCPIInfo(QString model, QString cmd, QString cmdtype, QString refname, QString reftype, QString unit);

    void appendSCPIInfo(QJsonArray &jsArr);

private:
    QString m_sSCPIModel;
    QString m_sSCPICmd;
    QString m_sSCPICmdType;
    QString m_sRefName;
    QString m_sRefType;
    QString m_sUnit;
};


#endif // SCPIINFO_H

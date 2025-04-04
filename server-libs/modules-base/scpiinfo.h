#ifndef SCPIINFO_H
#define SCPIINFO_H

#include <QString>
#include <QJsonArray>

namespace SCPI {
enum eSCPIEntryType {
    isComponent,
    isCatalog
};
}

class cSCPIInfo
{
public:

    cSCPIInfo(){}
    cSCPIInfo(QString model, QString cmd, QString cmdtype, QString refname, QString reftype, QString unit);
    cSCPIInfo(QString model, QString cmd, int cmdTypeMask, QString refname, SCPI::eSCPIEntryType entryType, QString unit);

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

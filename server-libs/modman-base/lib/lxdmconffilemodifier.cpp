#include "lxdmconffilemodifier.h"
#include <QFile>
#include <QSaveFile>

static const char* sessionLead = "last_lang=";

bool LxdmConfFileModifier::modifyConfig(const QString &sessionFileName, const QString &fileName)
{
    QFile configFileRead(fileName);
    if(!configFileRead.open(QFile::ReadOnly)) {
        qCritical("Cannot not read %s", qPrintable(sessionFileName));
        return false;
    }
    QString content = configFileRead.readAll();
    content.replace("\r", "");
    QStringList lines = content.split("\n");
    configFileRead.close();
    bool lastSessionFound = false;
    for(QString &line : lines) {
        if(line.startsWith(sessionLead)) {
            line = QString(sessionLead) + sessionFileName;
            lastSessionFound = true;
        }
    }
    if(!lastSessionFound) {
        qCritical("Cannot not find '%s' in %s!", sessionLead, qPrintable(sessionFileName));
        return false;
    }

    QSaveFile configFileWrite(fileName);
    if(!configFileWrite.open(QFile::WriteOnly)) {
        qCritical("Cannot not write %s", qPrintable(sessionFileName));
        return false;
    }
    for(const auto &line : lines)
        configFileWrite.write(line.toLatin1() + "\n");
    configFileWrite.commit();
    return true;
}

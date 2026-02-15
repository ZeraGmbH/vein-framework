#ifndef REGEXVALIDATOR_H
#define REGEXVALIDATOR_H

#include "validatorinterface.h"

class cRegExValidator: public ValidatorInterface
{
public:
    cRegExValidator(const QString &regex);
    cRegExValidator(const cRegExValidator& ref);
    bool isValidParam(QVariant& newValue) override;
    void exportMetaData(QJsonObject& jsObj) override;
    void setValidator(const QString &regex);
private:
    QString m_sRegEx;
};


#endif // REGEXVALIDATOR_H


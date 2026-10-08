#ifndef UINTVALIDATOR_H
#define UINTVALIDATOR_H

#include "validatorinterface.h"

class UintValidator : public ValidatorInterface
{
public:
    UintValidator();
    UintValidator(quint32 min, quint32 max, quint32 delta = 0);
    UintValidator(const UintValidator& ref);
    bool isValidParam(QVariant& newValue) override;
    void exportMetaData(QJsonObject& jsObj) override;
private:
    quint32 m_nMin;
    quint32 m_nMax;
    quint32 m_nDelta;
};

#endif // UINTVALIDATOR_H

#include "uintvalidator.h"
#include <QJsonArray>

UintValidator::UintValidator()
{
}

UintValidator::UintValidator(quint32 min, quint32 max, quint32 delta) :
    m_nMin(min),
    m_nMax(max),
    m_nDelta(delta)
{
}

UintValidator::UintValidator(const UintValidator &ref)
{
    m_nMin = ref.m_nMin;
    m_nMax = ref.m_nMax;
    m_nDelta = ref.m_nDelta;
}

bool UintValidator::isValidParam(QVariant &newValue)
{
    bool ok;
    quint32 value = newValue.toUInt(&ok);
    ok = ok && (value <= m_nMax) && (value >= m_nMin);
    if(ok)
        newValue = QVariant::fromValue<quint32>(value);
    return ok;
}

void UintValidator::exportMetaData(QJsonObject &jsObj)
{
    jsObj.insert("Type", "INTEGER");
    QJsonArray jsonArr = {qint64(m_nMin), qint64(m_nMax), qint64(m_nDelta)};
    jsObj.insert("Data", jsonArr);
}

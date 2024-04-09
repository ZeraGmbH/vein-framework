#include "testlicensesystem.h"

bool TestLicenseSystem::isSystemLicensed(const QString &uniqueModuleName)
{
    Q_UNUSED(uniqueModuleName)
    return true;
}

bool TestLicenseSystem::serialNumberIsInitialized() const
{
    return true;
}

void TestLicenseSystem::setDeviceSerial(const QString &serialNumber)
{
    if(!serialNumber.isEmpty())
        emit sigSerialNumberInitialized();
}

void TestLicenseSystem::processEvent(QEvent *event)
{
    Q_UNUSED(event)
}

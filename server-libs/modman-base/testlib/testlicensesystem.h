#ifndef TESTLICENSESYSTEM_H
#define TESTLICENSESYSTEM_H

#include <licensesysteminterface.h>

class TestLicenseSystem : public LicenseSystemInterface
{
    Q_OBJECT
public:
    void setDeviceSerial(const QString &serialNumber) override;
    bool isSystemLicensed(const QString &uniqueModuleName) override;
    bool serialNumberIsInitialized() const  override;
    void processEvent(QEvent *event) override;
};

#endif // TESTLICENSESYSTEM_H

#ifndef TESTLICENSESYSTEM_H
#define TESTLICENSESYSTEM_H

#include <licensesysteminterface.h>

class TestLicenseSystem : public LicenseSystemInterface
{
    Q_OBJECT
public:
    virtual bool isSystemLicensed(const QString &uniqueModuleName) override;
    virtual bool serialNumberIsInitialized() const  override;
    virtual void processEvent(QEvent *event) override;
};

#endif // TESTLICENSESYSTEM_H

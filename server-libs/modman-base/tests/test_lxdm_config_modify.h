#ifndef TEST_LXDM_CONFIG_MODIFY_H
#define TEST_LXDM_CONFIG_MODIFY_H

#include <QObject>
#include <QString>

class test_lxdm_config_modify : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void fileNotFound();
    void fileMissesSessionLine();
    void fileContainsSessionLine();
    void fileSessionSetValid();
private:
    void createTestConfigFile(QStringList lines);
};

#endif // TEST_LXDM_CONFIG_MODIFY_H

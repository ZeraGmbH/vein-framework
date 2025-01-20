#ifndef TEST_LXDM_CONFIG_FILE_H
#define TEST_LXDM_CONFIG_FILE_H

#include <QObject>
#include <QString>

class test_lxdm_config_modify : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void getCurrSessionFileNotFound();
    void getCurrSessionFileMissesSessionLine();
    void getCurrSessionFileContainsInValidSessionLine();
    void getCurrSessionFileContainsValidSessionLine();
    void getCurrSessionCorrectFound();
    void getCurrSessionFileContainsUnkownSessionLine();
    void getAvailableSessionsNoLxdmConfig();
    void getAvailableSessionsWithLxdmConfig();
    void getAvailableSessionsOneNorExisting();

    void setCurrSessionUnknown();
    void setCurrSessionValid();
    void setCurrSessionValidSessionFileNotExisting();
    void setCurrSessionValidDone();
    void setCurrSessionValidDoneSessionNotSet();
    void setCurrSessionValidDoneSessionSetTwice();
private:
    void createTestConfigFile(const QStringList &lines);
    void createTestSessionFiles(const QStringList &fileNames);
    QStringList m_sessionFilesGenerated;
};

#endif // TEST_LXDM_CONFIG_FILE_H

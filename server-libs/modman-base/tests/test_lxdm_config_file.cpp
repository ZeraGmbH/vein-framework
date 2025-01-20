#include "test_lxdm_config_file.h"
#include "lxdmconfigfile.h"
#include <QFile>
#include <QTest>

QTEST_MAIN(test_lxdm_config_modify)

static const char* testConfigFileName = "/tmp/lxdmConfTestFile";

void test_lxdm_config_modify::cleanup()
{
    QFile::remove(testConfigFileName);
    for(const QString &fileName : m_sessionFilesGenerated)
        QFile::remove(fileName);
}

void test_lxdm_config_modify::getCurrSessionFileNotFound()
{
    LxdmConfigFile confFile("foofile");
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
}

void test_lxdm_config_modify::getCurrSessionFileMissesSessionLine()
{
    createTestConfigFile(QStringList() << "" << "");
    LxdmConfigFile confFile(testConfigFileName);
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
}

void test_lxdm_config_modify::getCurrSessionFileContainsInValidSessionLine()
{
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() << LxdmConfigFile::XSession("Test", "test.desktop"));
    createTestConfigFile(QStringList() << "last_session test.desktop");
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
    createTestConfigFile(QStringList() << " last_session=test.desktop");
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
    createTestConfigFile(QStringList() << "last_session = test.desktop");
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
}

void test_lxdm_config_modify::getCurrSessionFileContainsValidSessionLine()
{
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop");
    createTestConfigFile(QStringList() << "last_session=/tmp/lxdm-foo.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() << LxdmConfigFile::XSession("Test", "/tmp/lxdm-foo.desktop"));
    QCOMPARE(confFile.getConfiguredXSessionName(), "Test");
}

void test_lxdm_config_modify::getCurrSessionCorrectFound()
{
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop" << "/tmp/lxdm-bar.desktop");
    createTestConfigFile(QStringList() << "last_session=/tmp/lxdm-bar.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");
}

void test_lxdm_config_modify::getCurrSessionFileContainsUnkownSessionLine()
{
    createTestConfigFile(QStringList() << "last_session=test.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() << LxdmConfigFile::XSession("Test", "foo.desktop"));
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
}

void test_lxdm_config_modify::getAvailableSessionsNoLxdmConfig()
{
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop" << "/tmp/lxdm-bar.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    QCOMPARE(confFile.getAvailableXSessionNames(), QStringList());
}

void test_lxdm_config_modify::getAvailableSessionsWithLxdmConfig()
{
    createTestConfigFile(QStringList() << "last_session=/tmp/lxdm-bar.desktop");
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop" << "/tmp/lxdm-bar.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    QCOMPARE(confFile.getAvailableXSessionNames(), QStringList() << "Foo" << "Bar");
}

void test_lxdm_config_modify::getAvailableSessionsOneNorExisting()
{
    createTestConfigFile(QStringList() << "last_session=/tmp/lxdm-bar.desktop");
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    QCOMPARE(confFile.getAvailableXSessionNames(), QStringList() << "Foo");
}

void test_lxdm_config_modify::setCurrSessionUnknown()
{
    createTestConfigFile(QStringList() << "last_session=foo.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() << LxdmConfigFile::XSession("Foo", "foo.desktop"));
    QCOMPARE(confFile.setCurrentXSession("foo"), false);
}

void test_lxdm_config_modify::setCurrSessionValid()
{
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop" << "/tmp/lxdm-bar.desktop");
    createTestConfigFile(QStringList() << "last_session=/tmp/lxdm-bar.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    QCOMPARE(confFile.setCurrentXSession("Bar"), true);
}

void test_lxdm_config_modify::setCurrSessionValidSessionFileNotExisting()
{
    createTestConfigFile(QStringList() << "last_session=bar.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "bar.desktop") );
    QCOMPARE(confFile.setCurrentXSession("Bar"), false);
}

void test_lxdm_config_modify::setCurrSessionValidDone()
{
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop" << "/tmp/lxdm-bar.desktop");
    createTestConfigFile(QStringList() << "last_session=/tmp/lxdm-foo.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    confFile.setCurrentXSession("Bar");
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");
}

void test_lxdm_config_modify::setCurrSessionValidDoneSessionNotSet()
{
    createTestConfigFile(QStringList());
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop" << "/tmp/lxdm-bar.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    confFile.setCurrentXSession("Bar");
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");
}

void test_lxdm_config_modify::setCurrSessionValidDoneSessionSetTwice()
{
    createTestSessionFiles(QStringList() << "/tmp/lxdm-foo.desktop" << "/tmp/lxdm-bar.desktop");
    createTestConfigFile(QStringList() << "last_session=/tmp/lxdm-foo.desktop" << "last_session=/tmp/lxdm-bar.desktop");
    LxdmConfigFile confFile(testConfigFileName,
                            QList<LxdmConfigFile::XSession>() <<
                                LxdmConfigFile::XSession("Foo", "/tmp/lxdm-foo.desktop") <<
                                LxdmConfigFile::XSession("Bar", "/tmp/lxdm-bar.desktop") );
    confFile.setCurrentXSession("Bar");
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");

    QFile configFileRead(testConfigFileName);
    configFileRead.open(QFile::ReadOnly);
    QString content = configFileRead.readAll();
    QCOMPARE(content, "last_session=/tmp/lxdm-bar.desktop\n");
}

void test_lxdm_config_modify::createTestConfigFile(QStringList lines)
{
    QFile file(testConfigFileName);
    file.open(QFile::WriteOnly);
    for(const auto &line : lines)
        file.write(line.toLatin1() + "\n");
}

void test_lxdm_config_modify::createTestSessionFiles(QStringList fileNames)
{
    for(const QString &fileName : fileNames) {
        QFile file(fileName);
        file.open(QFile::WriteOnly);
    }
    m_sessionFilesGenerated = fileNames;
}

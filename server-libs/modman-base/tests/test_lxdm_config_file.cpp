#include "test_lxdm_config_file.h"
#include "lxdmconfigfile.h"
#include "testlxdmconfigfilegenerator.h"
#include <QFile>
#include <QTest>

QTEST_MAIN(test_lxdm_config_modify)

static const char* testConfigFileName = "/tmp/lxdmConfTestFile";

void test_lxdm_config_modify::getCurrSessionFileNotFound()
{
    LxdmConfigFileParam param("foofile");
    LxdmConfigFile confFile(param);
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
}

void test_lxdm_config_modify::getCurrSessionFileMissesSessionLine()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << "" << "", QList<LxdmConfigFileParam::XSession>());
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
}

void test_lxdm_config_modify::getCurrSessionFileContainsInValidSessionLine()
{
    TestLxdmConfigFileGenerator paramGen(QStringList(),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Test", "test.desktop"));
    LxdmConfigFile confFile(paramGen.getConfigParam());

    TestLxdmConfigFileGenerator::createLxdmConfigFile(QStringList() << "last_session /tmp/lxdmtest/test.desktop");
    QCOMPARE(confFile.getConfiguredXSessionName(), "");

    TestLxdmConfigFileGenerator::createLxdmConfigFile(QStringList() << " last_session=/tmp/lxdmtest/test.desktop");
    QCOMPARE(confFile.getConfiguredXSessionName(), "");

    TestLxdmConfigFileGenerator::createLxdmConfigFile(QStringList() << "last_session = /tmp/lxdmtest/test.desktop");
    QCOMPARE(confFile.getConfiguredXSessionName(), "");

    TestLxdmConfigFileGenerator::createLxdmConfigFile(QStringList() << "last_session=/tmp/lxdmtest/test.desktop");
    QCOMPARE(confFile.getConfiguredXSessionName(), "Test");
}

void test_lxdm_config_modify::getCurrSessionFileContainsValidSessionLine()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("foo.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Test", "foo.desktop"));
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.getConfiguredXSessionName(), "Test");
}

void test_lxdm_config_modify::getCurrSessionCorrectFound()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("bar.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");
}

void test_lxdm_config_modify::getCurrSessionFileContainsUnkownSessionLine()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("unkown.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop"));
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.getConfiguredXSessionName(), "");
}

void test_lxdm_config_modify::getAvailableSessionsNoLxdmConfig()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("bar.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    paramGen.removeLxdmConfigFile();
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.getAvailableXSessionNames(), QStringList());
}

void test_lxdm_config_modify::getAvailableSessionsWithLxdmConfig()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("bar.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.getAvailableXSessionNames(), QStringList() << "Foo" << "Bar");
}

void test_lxdm_config_modify::getAvailableSessionsOneNotExisting()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("bar.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    paramGen.removeXsessionFile("bar.desktop");
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.getAvailableXSessionNames(), QStringList() << "Foo");
}

void test_lxdm_config_modify::setCurrSessionUnknown()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("foo.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") );
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.setCurrentXSession("Unknown"), false);
}

void test_lxdm_config_modify::setCurrSessionValid()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("foo.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.setCurrentXSession("Bar"), true);
}

void test_lxdm_config_modify::setCurrSessionValidSessionFileNotExisting()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("bar.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    paramGen.removeXsessionFile("bar.desktop");
    LxdmConfigFile confFile(paramGen.getConfigParam());
    QCOMPARE(confFile.setCurrentXSession("Bar"), false);
}

void test_lxdm_config_modify::setCurrSessionValidDone()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() << TestLxdmConfigFileGenerator::genLastSessionLine("foo.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    LxdmConfigFile confFile(paramGen.getConfigParam());
    confFile.setCurrentXSession("Bar");
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");
}

void test_lxdm_config_modify::setCurrSessionValidDoneSessionNotSet()
{
    TestLxdmConfigFileGenerator paramGen(QStringList(),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    LxdmConfigFile confFile(paramGen.getConfigParam());
    confFile.setCurrentXSession("Bar");
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");
}

void test_lxdm_config_modify::setCurrSessionValidDoneSessionSetTwice()
{
    TestLxdmConfigFileGenerator paramGen(QStringList() <<
                                             TestLxdmConfigFileGenerator::genLastSessionLine("foo.desktop") <<
                                             TestLxdmConfigFileGenerator::genLastSessionLine("bar.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    LxdmConfigFile confFile(paramGen.getConfigParam());
    confFile.setCurrentXSession("Bar");
    QCOMPARE(confFile.getConfiguredXSessionName(), "Bar");

    QFile configFileRead(TestLxdmConfigFileGenerator::getLxdmConfFileNameFull());
    configFileRead.open(QFile::ReadOnly);
    QString content = configFileRead.readAll();
    QCOMPARE(content, "last_session=/tmp/lxdmtest/bar.desktop\n");
}

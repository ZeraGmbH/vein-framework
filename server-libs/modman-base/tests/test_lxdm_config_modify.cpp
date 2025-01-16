#include "test_lxdm_config_modify.h"
#include "lxdmconffilemodifier.h"
#include <QFile>
#include <QTest>

QTEST_MAIN(test_lxdm_config_modify)

static const char* testConfigFileName = "/tmp/lxdmConfTestFile";

void test_lxdm_config_modify::cleanup()
{
    QFile file(testConfigFileName);
    file.remove();
}

void test_lxdm_config_modify::fileNotFound()
{
    QCOMPARE(LxdmConfFileModifier::modifyConfig("foosession", "foofile"), false);
}

void test_lxdm_config_modify::fileMissesSessionLine()
{
    createTestConfigFile(QStringList() << "" << "");
    QCOMPARE(LxdmConfFileModifier::modifyConfig("foosession", testConfigFileName), false);
}

void test_lxdm_config_modify::fileContainsSessionLine()
{
    createTestConfigFile(QStringList() << "last_lang");
    QCOMPARE(LxdmConfFileModifier::modifyConfig("foosession", testConfigFileName), false);
    createTestConfigFile(QStringList() << " last_lang=");
    QCOMPARE(LxdmConfFileModifier::modifyConfig("foosession", testConfigFileName), false);
    createTestConfigFile(QStringList() << "last_lang=");
    QCOMPARE(LxdmConfFileModifier::modifyConfig("foosession", testConfigFileName), true);
}

void test_lxdm_config_modify::fileSessionSetValid()
{
    QString line1 = "line1";
    QString line2 = "last_lang=";
    QString line3 = "line3";
    createTestConfigFile(QStringList() << line1 << line2 << line3);
    const QString newSession = "newSession";
    QCOMPARE(LxdmConfFileModifier::modifyConfig("newSession", testConfigFileName), true);

    QFile file(testConfigFileName);
    file.open(QFile::ReadOnly);
    QString bareLines = file.readAll();
    QStringList lines = bareLines.split("\n");
    QCOMPARE(lines[0], line1);
    QCOMPARE(lines[1], "last_lang=newSession");
    QCOMPARE(lines[2], line3);
}

void test_lxdm_config_modify::createTestConfigFile(QStringList lines)
{
    QFile file(testConfigFileName);
    file.open(QFile::WriteOnly);
    for(const auto &line : lines)
        file.write(line.toLatin1() + "\n");
}

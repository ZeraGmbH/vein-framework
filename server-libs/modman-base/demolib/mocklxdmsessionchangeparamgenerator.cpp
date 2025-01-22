#include "mocklxdmsessionchangeparamgenerator.h"
#include "mocklxdmconfigfilegenerator.h"
#include "lxdmsessionchangeparam.h"

LxdmSessionChangeParam MockLxdmSessionChangeParamGenerator::generateTestSessionChanger(bool restartServicePasses)
{
    MockLxdmConfigFileGenerator paramGen(QStringList() << MockLxdmConfigFileGenerator::genLastSessionLine("bar.desktop"),
                                         QList<LxdmConfigFileParam::XSession>() <<
                                             LxdmConfigFileParam::XSession("Foo", "foo.desktop") <<
                                             LxdmConfigFileParam::XSession("Bar", "bar.desktop") );
    return LxdmSessionChangeParam(paramGen.getConfigParam(), [=]() { return restartServicePasses; });
}

LxdmSessionChangeParam MockLxdmSessionChangeParamGenerator::generateDemoSessionChanger(bool restartServicePasses)
{
    LxdmConfigFileParam lxdmProductionParam;
    QList<LxdmConfigFileParam::XSession> docXsessionList;
    QString dummyXSessionFile;
    for(const auto &xsessionProduction : lxdmProductionParam.getAvailableXSessions()) {
        dummyXSessionFile = xsessionProduction.m_sessionName + ".desktop";
        LxdmConfigFileParam::XSession xSessionDoc = LxdmConfigFileParam::XSession(
            xsessionProduction.m_sessionName, dummyXSessionFile);
        docXsessionList.append(xSessionDoc);
    }
    MockLxdmConfigFileGenerator paramGen(QStringList() << MockLxdmConfigFileGenerator::genLastSessionLine(dummyXSessionFile),
                                         docXsessionList );
    return LxdmSessionChangeParam(paramGen.getConfigParam(), [=]() { return restartServicePasses; });
}

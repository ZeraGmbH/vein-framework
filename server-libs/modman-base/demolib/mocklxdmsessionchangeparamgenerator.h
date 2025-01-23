#ifndef MOCKLXDMSESSIONCHANGEPARAMGENERATOR_H
#define MOCKLXDMSESSIONCHANGEPARAMGENERATOR_H

#include "lxdmsessionchangeparam.h"

class MockLxdmSessionChangeParamGenerator
{
public:
    static LxdmSessionChangeParam generateTestSessionChanger(bool restartServicePasses = true);
    static LxdmSessionChangeParam generateDemoSessionChanger(bool restartServicePasses = true);
private:
    static bool handleRestart(bool restartServicePasses);
};

#endif // MOCKLXDMSESSIONCHANGEPARAMGENERATOR_H

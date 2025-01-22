#ifndef MOCKLXDMSESSIONCHANGEPARAMGENERATOR_H
#define MOCKLXDMSESSIONCHANGEPARAMGENERATOR_H

#include "lxdmsessionchangeparam.h"

class MockLxdmSessionChangeParamGenerator
{
public:
    static LxdmSessionChangeParam generateTestSessionChanger(bool restartServicePasses = true);
    static LxdmSessionChangeParam generateDemoSessionChanger(bool restartServicePasses = true);
};

#endif // MOCKLXDMSESSIONCHANGEPARAMGENERATOR_H

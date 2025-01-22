#include "lxdmsessionchangeparam.h"

LxdmSessionChangeParam::LxdmSessionChangeParam(const LxdmConfigFileParam &lxdmConfigParam,
                                               std::function<bool ()> restartService) :
    m_lxdmConfigParam(lxdmConfigParam),
    m_restartService(restartService)
{
}

LxdmConfigFileParam LxdmSessionChangeParam::getConfigParam() const
{
    return m_lxdmConfigParam;
}

std::function<bool ()> LxdmSessionChangeParam::getRestartFunc() const
{
    return m_restartService;
}

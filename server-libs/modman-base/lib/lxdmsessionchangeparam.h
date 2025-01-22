#ifndef LXDMSESSIONCHANGEPARAM_H
#define LXDMSESSIONCHANGEPARAM_H

#include "lxdmconfigfileparam.h"
#include <functional>

class LxdmSessionChangeParam
{
public:
    explicit LxdmSessionChangeParam(const LxdmConfigFileParam& lxdmConfigParam = LxdmConfigFileParam(),
                                    std::function<bool ()> restartService = []() {
                                        return system("systemctl restart lxdm.service") == 0;}
                                    );
    LxdmConfigFileParam getConfigParam() const;
    std::function<bool ()> getRestartFunc() const;
private:
    const LxdmConfigFileParam m_lxdmConfigParam;
    std::function<bool ()> m_restartService;
};

#endif // LXDMSESSIONCHANGEPARAM_H

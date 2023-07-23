#ifndef TCPWORKERFACTORYMETHODSTEST_H
#define TCPWORKERFACTORYMETHODSTEST_H

#include "vtcp_workerfactorymethods.h"

namespace VeinTcp
{

class TcpWorkerFactoryMethodsTest : public TcpWorkerFactoryMethods
{
public:
    static void enableProduction();
    static void enableMock();
};

}
#endif // TCPWORKERFACTORYMETHODSTEST_H

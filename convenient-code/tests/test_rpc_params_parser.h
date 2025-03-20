#ifndef TEST_RPC_PARAMS_PARSER_H
#define TEST_RPC_PARAMS_PARSER_H

#include <QObject>

class test_rpc_params_parser : public QObject
{
    Q_OBJECT
private slots:
    void parseNoParam();
    void parseIntSingleParam();
    void parseBoolSingleParam();
    void parseStringSingleParam();
    void parseStringListSingleParam();
    void parseDoubleNumberSingleParam();
    void parseFloatNumberSingleParam();

    void parseBoolBoolParam();
    void parseIntBoolParam();
};

#endif // TEST_RPC_PARAMS_PARSER_H

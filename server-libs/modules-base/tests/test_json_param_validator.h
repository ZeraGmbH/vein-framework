#ifndef TEST_JSON_PARAM_VALIDATOR_H
#define TEST_JSON_PARAM_VALIDATOR_H

#include <QObject>

class test_json_param_validator : public QObject
{
    Q_OBJECT
private slots:
    void validJson();
    void invalidJson();
    void invalidNoStructure();
    void validVariantMapToJson();
};

#endif // TEST_JSON_PARAM_VALIDATOR_H

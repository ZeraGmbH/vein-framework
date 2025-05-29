#include "test_json_param_validator.h"
#include "jsonparamvalidator.h"
#include <zera-jsonfileloader.h>
#include <QTest>

QTEST_MAIN(test_json_param_validator)

void test_json_param_validator::validJson()
{
    cJsonParamValidator validator;
    QJsonObject structure = cJsonFileLoader::loadJsonFile(":/test-structure.json");
    validator.setJSonParameterStructure(structure);

    ZeraJsonParamsState defaultStateGenerator(structure);
    QJsonObject json = defaultStateGenerator.getDefaultJsonState();
    QVariant variant(json);

    QCOMPARE(validator.isValidParam(variant), true);
}

void test_json_param_validator::invalidJson()
{
    cJsonParamValidator validator;
    QJsonObject structure = cJsonFileLoader::loadJsonFile(":/test-structure.json");
    validator.setJSonParameterStructure(structure);

    ZeraJsonParamsState defaultStateGenerator(structure);
    QJsonObject json = defaultStateGenerator.getDefaultJsonState();
    json["error"] = "foo";
    QVariant variant(json);

    QCOMPARE(validator.isValidParam(variant), false);
}

void test_json_param_validator::invalidNoStructure()
{
    cJsonParamValidator validator;

    QJsonObject structure = cJsonFileLoader::loadJsonFile(":/test-structure.json");
    ZeraJsonParamsState defaultStateGenerator(structure);
    QJsonObject json = defaultStateGenerator.getDefaultJsonState();
    QVariant variant(json);

    QCOMPARE(validator.isValidParam(variant), false);
}

void test_json_param_validator::validVariantMapToJson()
{
    cJsonParamValidator validator;
    QJsonObject structure = cJsonFileLoader::loadJsonFile(":/test-structure.json");
    validator.setJSonParameterStructure(structure);

    ZeraJsonParamsState defaultStateGenerator(structure);
    QVariantMap defaultValid = defaultStateGenerator.getDefaultJsonState().toVariantMap();
    QVariant variant(defaultValid);

    QCOMPARE(validator.isValidParam(variant), true);
    QCOMPARE(variant.type(), QMetaType::QJsonObject);
}

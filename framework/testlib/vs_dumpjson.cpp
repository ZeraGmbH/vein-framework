#include "vs_dumpjson.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

namespace VeinStorage
{

void DumpJson::dumpToFile(AbstractDatabase* db, QIODevice *outputFileDevice, QList<int> entityFilter)
{
    if((outputFileDevice->isOpen() || outputFileDevice->open(QIODevice::WriteOnly)) &&
        outputFileDevice->isWritable()) {
        QJsonObject rootObject;
        QList<int> tmpEntityIdKeys = db->getEntityList();
        std::sort(tmpEntityIdKeys.begin(), tmpEntityIdKeys.end());
        for(const int tmpEntityId : qAsConst(tmpEntityIdKeys)) {
            if(!entityFilter.isEmpty() && !entityFilter.contains(tmpEntityId))
                continue;
            EntityMap* entityMap = db->findEntity(tmpEntityId);;
            QJsonObject tmpEntityObject;

            QStringList tmpEntityComponentNames = db->getComponentList(tmpEntityId);
            std::sort(tmpEntityComponentNames.begin(), tmpEntityComponentNames.end());
            for(const QString &tmpComponentName : tmpEntityComponentNames) {
                QVariant tmpData = db->findComponent(entityMap, tmpComponentName)->getValue();
                QJsonValue toInsert;
                int tmpDataType = QMetaType::type(tmpData.typeName());
                if(tmpDataType == QMetaType::type("QList<int>")) { //needs manual conversion
                    QVariantList tmpIntList;
                    auto intList = tmpData.value<QList<int> >();
                    if(tmpEntityId == 0 && tmpComponentName == "Entities")
                        std::sort(intList .begin(), intList .end());
                    else
                        qFatal("Unexpected int-list: Entity id: %i / comonente %s", tmpEntityId, qPrintable(tmpComponentName));
                    for(const int &tmpInt : qAsConst(intList))
                        tmpIntList.append(tmpInt);
                    toInsert = QJsonArray::fromVariantList(tmpIntList);
                }
                else if(tmpDataType == QMetaType::type("QList<double>"))  { //needs manual conversion
                    QVariantList tmpDoubleList;
                    const auto doubleList = tmpData.value<QList<double> >();
                    for(const double tmpDouble : doubleList)
                        tmpDoubleList.append(tmpDouble);
                    toInsert = QJsonArray::fromVariantList(tmpDoubleList);
                }
                else if(tmpDataType == QMetaType::QStringList) { //needs manual conversion
                    QVariantList tmpStringList;
                    const auto stringList = tmpData.value<QStringList>();
                    for(const QString &tmpString : stringList)
                        tmpStringList.append(tmpString);
                    toInsert = QJsonArray::fromVariantList(tmpStringList);
                }
                else if(tmpData.canConvert(QMetaType::QVariantList) && tmpData.toList().isEmpty() == false)
                    toInsert = QJsonArray::fromVariantList(tmpData.toList());
                else if(tmpData.canConvert(QMetaType::QVariantMap) && tmpData.toMap().isEmpty() == false)
                    toInsert = QJsonObject::fromVariantMap(tmpData.toMap());
                else {
                    if(tmpComponentName == "INF_ModuleInterface" ) {
                        QJsonDocument doc = QJsonDocument::fromJson(tmpData.toString().toUtf8());
                        toInsert = doc.object();
                    }
                    else
                        toInsert = QJsonValue::fromVariant(tmpData);
                }
                if(!tmpData.isNull() && toInsert.isNull()) //how to consistently store and retrieve a QVector2D or QDateTime in JSON?
                    qWarning() << "Datatype" << tmpData.typeName() << "from" << tmpEntityId << tmpComponentName << "is not supported by function " << __PRETTY_FUNCTION__;
                tmpEntityObject.insert(tmpComponentName, toInsert);
            }
            rootObject.insert(QString::number(tmpEntityId), tmpEntityObject);
        }
        QJsonDocument tmpDoc;
        tmpDoc.setObject(rootObject);
        outputFileDevice->write(tmpDoc.toJson());
    }

    if(outputFileDevice->isOpen())
        outputFileDevice->close();

}

}

#include "vs_dumpjson.h"
#include <QBuffer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

namespace VeinStorage
{

void DumpJson::dumpToFile(AbstractDatabase* db, QIODevice *outputFileDevice, QList<int> entityFilter, QList<int> entitiesIgnored)
{
    if((outputFileDevice->isOpen() || outputFileDevice->open(QIODevice::WriteOnly)) &&
        outputFileDevice->isWritable()) {
        QJsonObject rootObject;
        QList<int> tmpEntityIdKeys = db->getEntityList();
        std::sort(tmpEntityIdKeys.begin(), tmpEntityIdKeys.end());
        for(const int tmpEntityId : qAsConst(tmpEntityIdKeys)) {
            if(!entityFilter.isEmpty() && !entityFilter.contains(tmpEntityId))
                continue;
            if(entitiesIgnored.contains(tmpEntityId))
                continue;
            QJsonObject tmpEntityObject;
            QStringList tmpEntityComponentNames = db->getComponentList(tmpEntityId);
            std::sort(tmpEntityComponentNames.begin(), tmpEntityComponentNames.end());
            for(const QString &tmpComponentName : tmpEntityComponentNames) {
                QVariant tmpData = db->getStoredValue(tmpEntityId, tmpComponentName);
                QJsonValue toInsert;
                int tmpDataType = QMetaType::type(tmpData.typeName());
                if(tmpDataType == QMetaType::type("QList<int>")) { //needs manual conversion
                    QVariantList tmpIntList;
                    auto intList = tmpData.value<QList<int> >();
                    for(const int &tmpInt : qAsConst(intList))
                        tmpIntList.append(tmpInt);
                    toInsert = QJsonArray::fromVariantList(tmpIntList);
                }
                else if(tmpDataType == QMetaType::type("QList<double>"))  { //needs manual conversion
                    QVariantList tmpDoubleList;
                    const auto doubleList = tmpData.value<QList<double> >();
                    for(const double tmpDouble : doubleList)
                        tmpDoubleList.append(formatDouble(tmpDouble));
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
                    else if(tmpDataType == QMetaType::Double) // force nan handled
                        toInsert = tmpData.toDouble();
                    else if(tmpDataType == QMetaType::Float) // force nan handled
                        toInsert = tmpData.toFloat();
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

QByteArray DumpJson::dumpToByteArray(AbstractDatabase *db, QList<int> entityFilter, QList<int> entitiesIgnored)
{
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    VeinStorage::DumpJson::dumpToFile(db, &buff, entityFilter, entitiesIgnored);
    return jsonDumped;
}

double DumpJson::formatDouble(double value)
{
    return QString("%1").arg(value, 0, 'g', 6).toDouble();
}

}

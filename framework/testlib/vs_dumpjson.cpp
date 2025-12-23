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
                QJsonValue toInsert = convertToJsonValue(tmpData);
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

QJsonValue DumpJson::convertToJsonValue(const QVariant &value)
{
    const QString typeName = value.typeName();
    // Qt5 handles empty string as null => check empty type name too
    if (value.isNull() && typeName.isEmpty())
        return QJsonValue();

    QJsonValue converted;
    int tmpDataType = QMetaType::type(value.typeName());
    if(tmpDataType == QMetaType::type("QList<int>")) { //needs manual conversion
        QVariantList tmpIntList;
        const auto intList = value.value<QList<int> >();
        for(const int &tmpInt : intList)
            tmpIntList.append(tmpInt);
        converted = QJsonArray::fromVariantList(tmpIntList);
    }
    else if(tmpDataType == QMetaType::type("QList<double>"))  { //needs manual conversion
        QVariantList tmpDoubleList;
        const auto doubleList = value.value<QList<double> >();
        for(const double tmpDouble : doubleList)
            tmpDoubleList.append(formatDouble(tmpDouble));
        converted = QJsonArray::fromVariantList(tmpDoubleList);
    }
    else if(tmpDataType == QMetaType::QStringList) { //needs manual conversion
        QVariantList tmpStringList;
        const auto stringList = value.value<QStringList>();
        for(const QString &tmpString : stringList)
            tmpStringList.append(tmpString);
        converted = QJsonArray::fromVariantList(tmpStringList);
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    else if(value.canConvert(QMetaType::QVariantList) && value.toList().isEmpty() == false)
        converted = QJsonArray::fromVariantList(value.toList());
    else if(value.canConvert(QMetaType::QVariantMap) && value.toMap().isEmpty() == false)
        converted = QJsonObject::fromVariantMap(value.toMap());
#endif
    else {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(value.toString().toUtf8(), &error);
        if (error.error == QJsonParseError::NoError)
            converted = doc.object();
        else if(tmpDataType == QMetaType::Double) // force nan handled
            converted = value.toDouble();
        else if(tmpDataType == QMetaType::Float) // force nan handled
            converted = value.toFloat();
        else
            converted = QJsonValue::fromVariant(value);
    }
    if(converted.isNull()) //how to consistently store and retrieve a QVector2D or QDateTime in JSON?
        qWarning() << "Datatype" << value.typeName() << "is not supported by function " << __PRETTY_FUNCTION__;
    return converted;
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

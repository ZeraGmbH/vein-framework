#ifndef VS_DUMPXML_H
#define VS_DUMPXML_H

#include "vfstorage_export.h"
#include "vs_abstractdatabase.h"
#include <QIODevice>
#include <QByteArray>
#include <QVariant>
#include <QList>
#include <QJsonValue>

namespace VeinStorage
{

class VFSTORAGE_EXPORT DumpJson
{
public:
    static void dumpToFile(const AbstractDatabase *db,
                           QIODevice *outputFileDevice,
                           const QList<int> &entityFilter = QList<int>(),
                           const QList<int> &entitiesIgnored = QList<int>(),
                           bool skipComponentDescription = false);
    static QJsonValue convertToJsonValue(const QVariant &value);
    static QByteArray dumpToByteArray(AbstractDatabase *db,
                                      const QList<int> &entityFilter = QList<int>(),
                                      const QList<int> &entitiesIgnored = QList<int>(),
                                      bool skipComponentDescription = false);
    static double formatDouble(double value);
private:
    static void skipDescrptionInModuleInterface(QJsonObject &tmpEntityObject, const QJsonValue &toInsert);
};

}
#endif // VS_DUMPXML_H

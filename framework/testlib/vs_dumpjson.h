#ifndef VS_DUMPXML_H
#define VS_DUMPXML_H

#include "vfstorage_export.h"
#include "vs_abstractdatabase.h"
#include <QIODevice>
#include <QByteArray>
#include <QList>

namespace VeinStorage
{

class VFSTORAGE_EXPORT DumpJson
{
public:
    static void dumpToFile(AbstractDatabase *db, QIODevice *outputFileDevice,
                           QList<int> entityFilter = QList<int>(),
                           QList<int> entitiesIgnored = QList<int>());
    static QByteArray dumpToByteArray(AbstractDatabase *db,
                                      QList<int> entityFilter = QList<int>(),
                                      QList<int> entitiesIgnored = QList<int>());
    static double formatDouble(double value);
};

}
#endif // VS_DUMPXML_H

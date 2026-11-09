#ifndef TEST_NETWORKSYSTEM_DESERIALIZE_H
#define TEST_NETWORKSYSTEM_DESERIALIZE_H

#include <QObject>

class test_networksystem_deserialize : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void wellFormedEnvelopeIsForwarded();
    void envelopeWithoutEventVectorIsIgnored();
    void eventWithoutEventDataIsIgnored();
    void eventWithoutEventDataDoesNotDropOtherEvents();
};

#endif // TEST_NETWORKSYSTEM_DESERIALIZE_H

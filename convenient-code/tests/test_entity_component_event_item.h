#ifndef TEST_CMD_EVENT_ITEM_ENTITY_H
#define TEST_CMD_EVENT_ITEM_ENTITY_H

#include <QObject>

class test_entity_component_event_item : public QObject
{
    Q_OBJECT
private slots:
    void ignoreOtherComponentSameEntity();
    void acceptSameComponentSameEntity();
    void sameComponentNamesOnDifferentEntities();
};

#endif // TEST_CMD_EVENT_ITEM_ENTITY_H

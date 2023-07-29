#ifndef TEST_CMD_EVENT_ITEM_ENTITY_H
#define TEST_CMD_EVENT_ITEM_ENTITY_H

#include <QObject>

class test_cmd_event_item_entity : public QObject
{
    Q_OBJECT
private slots:
    void ignoreOtherComponentSameEntity();
    void acceptSameComponentSameEntity();
    void sameComponentNamesOnDifferentEntities();
};

#endif // TEST_CMD_EVENT_ITEM_ENTITY_H

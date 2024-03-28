#ifndef TESTCOMMANDEVENTSTRINGS_H
#define TESTCOMMANDEVENTSTRINGS_H

#include "ve_commandevent.h"
#include <vcmp_entitydata.h>
#include <vcmp_componentdata.h>

class TestCommandEventStrings
{
public:
    static QString strSubtype(VeinEvent::CommandEvent::EventSubtype subtype);
    static QString strOrigin(qint8 origin);
    static QString strTarget(qint8 target);
    static QString strEntityCommand(VeinComponent::EntityData::Command cmd);
    static QString strComponentCommand(VeinComponent::ComponentData::Command cmd);
};

#endif // TESTCOMMANDEVENTSTRINGS_H

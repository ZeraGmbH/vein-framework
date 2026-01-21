#ifndef ERRORDATASENDER_H
#define ERRORDATASENDER_H

#include "ve_eventsystem.h"
#include <QEvent>

class ErrorDataSender
{
public:
    static void errorOut(const QString &errorMessage, QEvent* event, VeinEvent::EventSystem *eventSystem);
};

#endif // ERRORDATASENDER_H

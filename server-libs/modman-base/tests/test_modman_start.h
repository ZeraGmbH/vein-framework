#ifndef TEST_MODMAN_START_H
#define TEST_MODMAN_START_H

#include <QObject>

class test_modman_start : public QObject
{
    Q_OBJECT
private slots:
    void emptyModman();
    void modmanPlusOneEntity();
    void modmanPlusTwoEntities();
    void modmanPlusOneEntityModulesLoaded();
};

#endif // TEST_MODMAN_START_H

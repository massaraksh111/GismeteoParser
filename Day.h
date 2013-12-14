#ifndef DAY_H
#define DAY_H

#include <QPair>
#include <QList>

struct Half
{
    int t;  // температура
    int p;  // давление
    int cl; // облачность
    int ph; // явления
    int ws; // ветер скорость
    int wd; // ветер направление
};

typedef QList< QPair<Half, Half> > Day;

#endif // DAY_H

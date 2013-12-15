#ifndef DAY_H
#define DAY_H

#include <QString>
#include <QPair>
#include <QList>

enum Cloudness
{
    Sun,
    Sunc,
    Suncl,
    Dull
};

enum Precipitations
{
    None,
    Rain,
    Snow,
    Storm,
};

inline QString windLable(QString str)
{
    if(str == "С") return "N";
    else if(str == "СВ") return "NE";
    else if(str == "В") return "E";
    else if(str == "ЮВ") return "SE";
    else if(str == "Ю") return "S";
    else if(str == "ЮЗ") return "SW";
    else if(str == "З") return "W";
    else if(str == "СЗ") return "NW";
    else return "C";
}

struct Half
{
    int t;  // температура
    int p;  // давление
    Cloudness cl; // облачность
    Precipitations ph; // явления
    int ws; // ветер скорость
    QString wd; // ветер направление
};

typedef QList< QPair<Half, Half> > Month;

#endif // DAY_H

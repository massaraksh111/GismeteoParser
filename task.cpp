#include "task.h"
#include "day.h"

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtConcurrent/QtConcurrentMap>
#include <QFile>
#include <QTextStream>

QString& Bad() { static QString t; return t; }

Task::Task(int index, QPair<int, int> from, QPair<int, int> to)
    : QObject(0), _canselled(false)
{
    _index = index;
    _from = from;
    _to = to;

    _network = new QNetworkAccessManager(this);
    connect(_network, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

Task::~Task()
{
    delete _network;
}

void Task::start()
{
    QString str = QString("http://www.gismeteo.ru/diary/%1/%3/%2/")
            .arg(_index).arg(_from.first).arg(_from.second);

    _network->get(QNetworkRequest(QUrl(str)));
}

void Task::stop()
{
    _canselled = true;
    disconnect(_network, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(replyFinished(QNetworkReply*)));
    start();
}

void Task::replyFinished(QNetworkReply* reply)
{
    QString str = QString("%1 %2-%3").arg(_index).arg(_from.first--).arg(_from.second);
    _data[str] = reply;

    emit log(_index, QString("Ответ %1 получен").arg(str));

    if(_from.first < 0)
    {
        _from.second--;
        _from.first = 12;
    }

    QDate from(_from.second, _from.first, 1);
    QDate to(_to.second, _to.first, 1);

    if(from < to)
    {
        emit log(_index, "Данные получены");
        parseReplies();
    }
    else if(_canselled)
    {
        emit log(_index, "Отменено");
        emit finished();
    }
    else
    {
        start();
    }
}

int TP(QString str)
{
    int from = str.indexOf(">")+1;
    int to   = str.indexOf("<");
    return str.mid(from, to - from).toInt();
}

Cloudness cloudness(QString str)
{
    static QStringList list = QStringList() << "sun" << "sunc" << "suncl" << "dull";

    for(int i = 0; i < list.size(); i++)
        if(str.indexOf(list[i]) != -1)
            return Cloudness(i);

    return Cloudness(0);
}

QPair<QString, int> wind(QString str)
{
    int from = str.indexOf("<br />") + 6;
    int count = str.indexOf("м/с") - from;
    int none = str.indexOf("Ш");
    if(none == -1)
    {
        auto wind = str.mid( from, count ).split(" ", QString::SkipEmptyParts);
        return qMakePair(wind[0], wind[1].toInt());
    }
    else
        return qMakePair(QString("Ш"), 0);
}

Precipitations precipitations(QString str)
{
    static QStringList list = QStringList() << "rain" << "snow" << "storm";

    for(int i = 0; i < list.size(); i++)
        if(str.indexOf(list[i]) != -1)
            return Precipitations(i+1);
    return None;
}

Month parseSingleReply(QNetworkReply* reply, QString& error)
{
    QTextStream page(reply);

    QString str = page.readAll();

    Month ret;
    if(str.indexOf("Ошибка") != -1)
    {
        error = "Ошибка в запросе";
        return ret;
    }

    int start = str.indexOf("<tbody>");
    int end = str.indexOf("</tbody>");

    str = str.mid(start, end - start);

    auto list = str.split("<tr align=\"center\">", QString::SkipEmptyParts);

    for(int i = 1; i < list.size(); i++)
    {
        QString line = list[i];
        auto day = line.split("<td", QString::SkipEmptyParts);

        Half first;
        Half second;
        first.t = TP(day[2]);
        second.t = TP(day[7]);

        first.p = TP(day[3]);
        second.p = TP(day[8]);

        first.cl = cloudness(day[4]);
        second.cl = cloudness(day[9]);

        first.ph = precipitations(day[5]);
        second.ph = precipitations(day[10]);

        auto windAM = wind(day[6]);
        auto windPM = wind(day[11]);

        first.wd = windLable(windAM.first);
        second.wd = windLable(windPM.first);

        first.ws = windAM.second;
        second.ws = windPM.second;

        ret.append( qMakePair(first, second) );
    }

    return ret;
}

void mapPair(QPair<QString, QNetworkReply*> it)
{
    QString err;
    Month monthArr = parseSingleReply(it.second, err);

    QFile file(it.first + ".txt");
    if(!file.open(QFile::WriteOnly))
    {
        Bad() = "Не могу открыть файл";
        return;
    }

    int space = it.first.indexOf(" ");
    int defis = it.first.indexOf("-");

    int index = it.first.mid(0, space).toInt();
    int month = it.first.mid(space+1, defis - space - 1).toInt();
    int year = it.first.mid(defis+1).toInt();

    QTextStream out(&file);

    if(!err.isEmpty())
    {
        out << err;
    }
    else
    {
        for(int i = 0; i < monthArr.size(); i++)
        {
            auto today = monthArr[i];
            out << index << ','
                << year << ','
                << month << ','
                << i+1 << ','
                << today.first.t << ','
                << today.first.p << ','
                << today.first.cl << ','
                << today.first.ph << ','
                << today.first.ws << ','
                << today.first.wd << ','
                << today.second.t << ','
                << today.second.p << ','
                << today.second.cl << ','
                << today.second.ph << ','
                << today.second.ws << ','
                << today.second.wd << '\n';
        }
    }
    out.flush();
}

void Task::parseReplies()
{
    log(_index, "Начат парсинг");
    QList<QPair<QString, QNetworkReply*>> data;
    for(auto i = _data.begin(); i != _data.end(); i++)
        data.append(qMakePair(i.key(), i.value()));

    QtConcurrent::blockingMap(data, mapPair);

    if(!Bad().isEmpty())
        emit log(_index, Bad());

    emit log(_index, "Парсинг закончен");
    emit finished();
}

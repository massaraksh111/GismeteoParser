#ifndef TASK_H
#define TASK_H

#include <QNetworkAccessManager>
#include <QRunnable>
#include <QString>
#include <QPair>
#include <QMap>

class Task : public QRunnable
{
    Q_OBJECT

    int _index;
    QPair<int, int> _from;
    QPair<int, int> _to;

    QMap<QString, QString> _data;

    QNetworkAccessManager* _network;

public:
    Task(int index, QPair<int, int> from, QPair<int, int> to);
    virtual ~Task();
    void run();

signals:
    void

};

#endif // TASK_H

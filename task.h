#ifndef TASK_H
#define TASK_H

#include <QNetworkAccessManager>
#include <QRunnable>
#include <QString>
#include <QPair>
#include <QMap>

class Task : public QObject
{
    Q_OBJECT

public:
    typedef QMap<QString, QNetworkReply*>::iterator iterator;

private:
    int _index;
    QPair<int, int> _from;
    QPair<int, int> _to;

    QPair<int, int> _current;

    QMap<QString, QNetworkReply*> _data;

    QNetworkAccessManager* _network;

    bool _canselled;

public:
    Task(int index, QPair<int, int> from, QPair<int, int> to);
    virtual ~Task();

    int index() const { return _index; }
signals:
    void finished();
    void log(int, QString);

public slots:
    void start();
    void stop();

private slots:
    void replyFinished(QNetworkReply*);
    void parseReplies();

};

#endif // TASK_H

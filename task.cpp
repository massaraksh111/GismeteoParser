#include "task.h"
#include <QNetworkRequest>

Task::Task(int index, QPair<int, int> from, QPair<int, int> to)
{
    _index = index;
    _from = from;
    _to = to;

    _network = new QNetworkAccessManager(this);
}

Task::~Task()
{
    delete _network;
}

void Task::run()
{
    _network->get(QNetworkRequest(QUrl("")));
}

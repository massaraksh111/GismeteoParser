#include "window.h"

#include <QNetworkAccessManager>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QLabel>

Window::Window(QWidget *parent) :
    QWidget(parent), _working(false)
{
    _label = new QLabel("Номера страниц через пробелы", this);
    _text = new QLineEdit(this);
    _button = new QPushButton("Начать", this);
    _edit = new QTextEdit(this);
    _from = new QDateEdit(QDate::currentDate(), this);
    _to = new QDateEdit(QDate::currentDate(), this);

    _from->setDisplayFormat("MM.yyyy");
    _to->setDisplayFormat("MM.yyyy");


    auto line = new QHBoxLayout();
    line->addWidget(_label);
    line->addWidget(_text);

    auto grid = new QGridLayout();
    grid->addLayout(line, 0, 0, 1, 4);

    grid->addWidget(new QLabel("Начало", this), 1, 0, 1, 1);
    grid->addWidget(_from, 1, 1, 1, 1);
    grid->addWidget(new QLabel("Конец", this), 1, 2, 1, 1);
    grid->addWidget(_to, 1, 3, 1, 1);

    grid->addWidget(_button, 2, 3, 1, 1);
    grid->addWidget(_edit, 3, 0, 1, 4);

    this->setLayout(grid);

    connect(_button, SIGNAL(clicked()), this, SLOT(startParsing()));
}

void Window::log(int index, QString str)
{
    QString text = QString("%1%4: %2 -> %3\n").arg(_edit->toPlainText()).arg(index)
                        .arg(str).arg(QTime::currentTime().toString());
    QTextCursor tmpCursor = _edit->textCursor();

    _edit->setText(text);
    tmpCursor.movePosition(QTextCursor::End);

    _edit->setTextCursor(tmpCursor);
}

void Window::startParsing()
{
    if(_text->text().isEmpty())
        return;

    _edit->setPlainText("");
    _button->setText("Отменить");

    disconnect(_button, SIGNAL(clicked()), this, SLOT(startParsing()));
    connect(_button, SIGNAL(clicked()), this, SLOT(stopParsing()));

    auto list = _text->text().split(" ", QString::SkipEmptyParts);

    for(int i = 0; i < _tasks.size(); i++)
    {
        _threads[i]->deleteLater();
        _tasks[i]->deleteLater();
    }

    _tasks.clear();
    _threads.clear();

    for(int i = 0; i < list.size(); i++)
    {
        int index = list[i].toInt();
        log(index, "Начало");
        _threads.append(new QThread());
        _tasks.append(new Task(index, qMakePair(_to->date().month(), _to->date().year()),
                                      qMakePair(_from->date().month(), _from->date().year())));

        connect(_threads.back(), SIGNAL(started()), _tasks.back(), SLOT(start()));
        connect(_tasks.back(), SIGNAL(finished()), _threads.back(), SLOT(quit()));
        connect(_tasks.back(), SIGNAL(finished()), this, SLOT(finished()));
        connect(_tasks.back(), SIGNAL(log(int,QString)), this, SLOT(log(int,QString)));

       _tasks.back()->moveToThread(_threads.back());
    }

    for(int i = 0; i < list.size(); i++)
    {
        _threads[i]->start();
    }
}

void Window::stopParsing()
{
    finished();

    for(int i = 0; i < _tasks.size(); i++)
    {
        _tasks[i]->stop();
    }
}

void Window::finished()
{
    _button->setText("Начать");

    disconnect(_button, SIGNAL(clicked()), this, SLOT(stopParsing()));
    connect(_button, SIGNAL(clicked()), this, SLOT(startParsing()));

}

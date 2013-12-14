#include "window.h"

#include <QNetworkAccessManager>
#include <QProgressBar>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

Window::Window(QWidget *parent) :
    QWidget(parent), _working(false)
{
    _label = new QLabel("Номера страниц через пробелы", this);
    _text = new QLineEdit(this);
    _button = new QPushButton("Начать", this);
    _bar = new QProgressBar(this);
    _bar->setStyleSheet("QProgressBar {"
                       "text-align: center;"
                       "}");


    auto line = new QHBoxLayout();
    line->addWidget(_label);
    line->addWidget(_text);

    auto grid = new QGridLayout();
    grid->addLayout(line, 0, 0, 1, 3);
    grid->addWidget(_button, 1, 2, 1, 1);
    grid->addWidget(_bar, 2, 0, 1, 3);

    this->setLayout(grid);
}

void Window::startParsing()
{
    _button->setText("Отменить");
}

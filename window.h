#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QThread>

#include "task.h"

class QLabel;
class QLineEdit;
class QTextEdit;
class QDateEdit;
class QPushButton;
class QNetworkAccessManager;

class Window : public QWidget
{
    Q_OBJECT

    QLabel* _label;
    QLineEdit* _text;
    QPushButton* _button;
    QTextEdit* _edit;
    QDateEdit* _from;
    QDateEdit* _to;

    bool _working;

    QList<QThread*> _threads;
    QList<Task*> _tasks;

public:
    explicit Window(QWidget *parent = 0);
    
signals:

public slots:
    void log(int index, QString str);
    
private slots:
    void startParsing();
    void stopParsing();
    void finished();
};

#endif // WINDOW_H

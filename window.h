#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QNetworkAccessManager;

class Window : public QWidget
{
    Q_OBJECT

    QLabel* _label;
    QLineEdit* _text;
    QPushButton* _button;
    QProgressBar* _bar;

    bool _working;

public:
    explicit Window(QWidget *parent = 0);
    
signals:
    
private slots:
    void startParsing();
};

#endif // WINDOW_H

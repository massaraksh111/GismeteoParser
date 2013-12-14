#include <QApplication>
#include "window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    auto wind = new Window(0);

    wind->show();

    return app.exec();
}

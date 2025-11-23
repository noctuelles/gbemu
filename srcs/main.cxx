//
// Created by plouvel on 12/8/24.
//

#include <QApplication>

#include "ui/MainWindow.hxx"

int main(int argc, char* args[])
{
    QApplication app{argc, args};
    MainWindow window{};

    window.show();

    return app.exec();
}
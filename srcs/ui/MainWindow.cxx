//
// Created by plouvel on 23/11/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ui/MainWindow.hxx"

#include <iostream>

#include "Emulator.hxx"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _renderer(new QtRenderer), _emulator(_renderer, this), _ui(new Ui::MainWindow)
{
    connect(_renderer, &QtRenderer::frameReady, this, &MainWindow::onFrameReady);

    _emulator.start();

    _ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::onFrameReady()
{
    _ui->display->setPixmap(QPixmap::fromImage(
        _renderer->render().scaled(_ui->display->size(), Qt::IgnoreAspectRatio, Qt::FastTransformation)));
}

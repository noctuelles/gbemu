//
// Created by plouvel on 12/4/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Debugger.h" resolved

#include "ui/Debugger.hxx"

#include "ui_Debugger.h"

Debugger::Debugger(QWidget* parent) : QMainWindow(parent), ui(new Ui::Debugger)
{
    ui->setupUi(this);
}

Debugger::~Debugger()
{
    delete ui;
}

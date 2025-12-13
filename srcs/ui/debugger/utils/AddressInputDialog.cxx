//
// Created by plouvel on 12/13/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddressInputDialog.h" resolved

#include "ui/debugger/utils/AddressInputDialog.hxx"

#include <QRegularExpressionValidator>

#include "ui_AddressInputDialog.h"

AddressInputDialog::AddressInputDialog(const QString& title, const int maxSize, QWidget* parent)
    : QDialog(parent), ui(new Ui::AddressInputDialog)
{
    ui->setupUi(this);
    ui->address->setValidator(
        new QRegularExpressionValidator(QRegularExpression(QStringLiteral("^[0-9A-Fa-f]{0,%1}$").arg(maxSize)),
                                        ui->address));

    setWindowTitle(title);
}

AddressInputDialog::~AddressInputDialog()
{
    delete ui;
}

uint64_t AddressInputDialog::getAddress() const
{
    if (ui->address->text().isEmpty())
    {
        return 0;
    }

    return ui->address->text().toULongLong(nullptr, 16);
}
//
// Created by plouvel on 11/29/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Preference.h" resolved

#include "Preference.hxx"

#include <QFileDialog>
#include <QSettings>

#include "ui_Preference.h"

Preference::Preference(QWidget* parent) : QDialog(parent), _ui(new Ui::Preference)
{
    QSettings settings{};

    _ui->setupUi(this);

    connect(_ui->enableBootRomCheckbox, &QCheckBox::toggled, this,
            [this](const bool checked) { _ui->pickBootRom->setEnabled(checked); });
    connect(_ui->pickBootRom, &QPushButton::clicked, this,
            [this]
            {
                const auto path =
                    QFileDialog::getOpenFileName(this, tr("Select Boot ROM"), ".", tr("ROM Files (*.gb)"));

                if (!path.isEmpty())
                {
                    _bootRomPath = path;
                }
            });

    settings.beginGroup("preference/keys");

    _ui->upKeySequenceEdit->setKeySequence(QKeySequence{settings.value("up", QKeySequence{}).toString()});
    _ui->downKeySequenceEdit->setKeySequence(QKeySequence{settings.value("down", QKeySequence{}).toString()});
    _ui->leftKeySequenceEdit->setKeySequence(QKeySequence{settings.value("left", QKeySequence{}).toString()});
    _ui->rightKeySequenceEdit->setKeySequence(QKeySequence{settings.value("right", QKeySequence{}).toString()});

    _ui->aKeySequenceEdit->setKeySequence(QKeySequence{settings.value("a", QKeySequence{}).toString()});
    _ui->bKeySequenceEdit->setKeySequence(QKeySequence{settings.value("b", QKeySequence{}).toString()});
    _ui->selectKeySequenceEdit->setKeySequence(QKeySequence{settings.value("select", QKeySequence{}).toString()});
    _ui->startKeySequenceEdit->setKeySequence(QKeySequence{settings.value("start", QKeySequence{}).toString()});

    settings.endGroup();

    _ui->enableBootRomCheckbox->setChecked(settings.value("preference/system/enableBootRom", false).toBool());
    _bootRomPath = settings.value("preference/system/bootRomPath", QString()).toString();
}

Preference::~Preference()
{
    delete _ui;
}

void Preference::accept()
{
    QSettings settings{};

    settings.setValue("preference/system/enableBootRom", _ui->enableBootRomCheckbox->isChecked());
    settings.setValue("preference/system/bootRomPath", _bootRomPath);

    settings.beginGroup("preference/keys");

    settings.setValue("up", _ui->upKeySequenceEdit->keySequence().toString());
    settings.setValue("down", _ui->downKeySequenceEdit->keySequence().toString());
    settings.setValue("left", _ui->leftKeySequenceEdit->keySequence().toString());
    settings.setValue("right", _ui->rightKeySequenceEdit->keySequence().toString());

    settings.setValue("a", _ui->aKeySequenceEdit->keySequence().toString());
    settings.setValue("b", _ui->bKeySequenceEdit->keySequence().toString());
    settings.setValue("select", _ui->selectKeySequenceEdit->keySequence().toString());
    settings.setValue("start", _ui->startKeySequenceEdit->keySequence().toString());

    settings.endGroup();

    QDialog::accept();
}

void Preference::reject()
{
    QDialog::reject();
}

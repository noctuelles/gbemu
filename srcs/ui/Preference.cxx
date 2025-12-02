//
// Created by plouvel on 11/29/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Preference.h" resolved

#include "../../includes/ui/Preference.hxx"

#include <QFileDialog>
#include <QMessageBox>
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

    _ui->upKeySequenceEdit->setKeySequence(settings.value("up").value<QKeySequence>());
    _ui->downKeySequenceEdit->setKeySequence(settings.value("down").value<QKeySequence>());
    _ui->leftKeySequenceEdit->setKeySequence(settings.value("left").value<QKeySequence>());
    _ui->rightKeySequenceEdit->setKeySequence(settings.value("right").value<QKeySequence>());

    _ui->aKeySequenceEdit->setKeySequence(settings.value("a").value<QKeySequence>());
    _ui->bKeySequenceEdit->setKeySequence(settings.value("b").value<QKeySequence>());
    _ui->selectKeySequenceEdit->setKeySequence(settings.value("select").value<QKeySequence>());
    _ui->startKeySequenceEdit->setKeySequence(settings.value("start").value<QKeySequence>());

    settings.endGroup();

    const QList keySequenceEdits{
        _ui->upKeySequenceEdit, _ui->downKeySequenceEdit, _ui->leftKeySequenceEdit,   _ui->rightKeySequenceEdit,
        _ui->aKeySequenceEdit,  _ui->bKeySequenceEdit,    _ui->selectKeySequenceEdit, _ui->startKeySequenceEdit,
    };

    auto checkKeySequenceEditChanged = [this, keySequenceEdits](const QKeySequence& sequence)
    {
        const auto senderKeySequenceEdit{qobject_cast<QKeySequenceEdit*>(sender())};

        for (const auto keySequenceEdit : keySequenceEdits)
        {
            if (keySequenceEdit->keySequence() == sequence && keySequenceEdit != senderKeySequenceEdit)
            {
                senderKeySequenceEdit->clear();
                return;
            }
        }
    };

    for (const auto keySequenceEdit : keySequenceEdits)
    {
        connect(keySequenceEdit, &QKeySequenceEdit::keySequenceChanged, this, checkKeySequenceEditChanged);
    }

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

    settings.setValue("up", _ui->upKeySequenceEdit->keySequence());
    settings.setValue("down", _ui->downKeySequenceEdit->keySequence());
    settings.setValue("left", _ui->leftKeySequenceEdit->keySequence());
    settings.setValue("right", _ui->rightKeySequenceEdit->keySequence());

    settings.setValue("a", _ui->aKeySequenceEdit->keySequence());
    settings.setValue("b", _ui->bKeySequenceEdit->keySequence());
    settings.setValue("select", _ui->selectKeySequenceEdit->keySequence());
    settings.setValue("start", _ui->startKeySequenceEdit->keySequence());

    settings.endGroup();

    QDialog::accept();
}

void Preference::reject()
{
    QDialog::reject();
}

//
// Created by plouvel on 11/29/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Preference.h" resolved

#include "../../includes/ui/Preference.hxx"

#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "ui/Settings.hxx"
#include "ui_Preference.h"

Preference::Preference(QWidget* parent) : QDialog(parent), _ui(new Ui::Preference)
{
    QSettings settings{};

    _ui->setupUi(this);

    {
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

        _ui->enableBootRomCheckbox->setChecked(settings.value("preference/system/enableBootRom", false).toBool());
        _bootRomPath = settings.value("preference/system/bootRomPath", QString()).toString();
    }

    {
        using namespace Settings::Keys;

        _ui->upKeySequenceEdit->setKeySequence(get(Key::Up));
        _ui->downKeySequenceEdit->setKeySequence(get(Key::Down));
        _ui->leftKeySequenceEdit->setKeySequence(get(Key::Left));
        _ui->rightKeySequenceEdit->setKeySequence(get(Key::Right));

        _ui->aKeySequenceEdit->setKeySequence(get(Key::A));
        _ui->bKeySequenceEdit->setKeySequence(get(Key::B));
        _ui->selectKeySequenceEdit->setKeySequence(get(Key::Select));
        _ui->startKeySequenceEdit->setKeySequence(get(Key::Start));

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
    }

    {
        using namespace Settings::Palette;

        const auto setBtnBackgroundColor = [](QPushButton* button, const QColor& color)
        {
            QPalette palette{};

            palette.setColor(QPalette::Button, color);

            button->setAutoFillBackground(true);
            button->setPalette(palette);
            button->update();
        };

        const auto updateColorPicker = [this, setBtnBackgroundColor]
        {
            const auto button{qobject_cast<QPushButton*>(sender())};

            const auto color = QColorDialog::getColor(button->palette().color(QPalette::Button), this,
                                                      tr("Select Color"), QColorDialog::DontUseNativeDialog);

            if (color.isValid())
            {
                setBtnBackgroundColor(button, color);
            }
        };

        setBtnBackgroundColor(_ui->color0, get(Type::Color0));
        connect(_ui->color0, &QPushButton::clicked, this, updateColorPicker);

        setBtnBackgroundColor(_ui->color1, get(Type::Color1));
        connect(_ui->color1, &QPushButton::clicked, this, updateColorPicker);

        setBtnBackgroundColor(_ui->color2, get(Type::Color2));
        connect(_ui->color2, &QPushButton::clicked, this, updateColorPicker);

        setBtnBackgroundColor(_ui->color3, get(Type::Color3));
        connect(_ui->color3, &QPushButton::clicked, this, updateColorPicker);

        connect(_ui->resetPalette, &QPushButton::clicked, this,
                [this, setBtnBackgroundColor]
                {
                    setBtnBackgroundColor(_ui->color0, DEFAULT_COLORS[0]);
                    setBtnBackgroundColor(_ui->color1, DEFAULT_COLORS[1]);
                    setBtnBackgroundColor(_ui->color2, DEFAULT_COLORS[2]);
                    setBtnBackgroundColor(_ui->color3, DEFAULT_COLORS[3]);
                });
    }
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

    {
        using namespace Settings::Keys;

        set(Key::Up, _ui->upKeySequenceEdit->keySequence());
        set(Key::Down, _ui->downKeySequenceEdit->keySequence());
        set(Key::Left, _ui->leftKeySequenceEdit->keySequence());
        set(Key::Right, _ui->rightKeySequenceEdit->keySequence());
        set(Key::A, _ui->aKeySequenceEdit->keySequence());
        set(Key::B, _ui->bKeySequenceEdit->keySequence());
        set(Key::Select, _ui->selectKeySequenceEdit->keySequence());
        set(Key::Start, _ui->startKeySequenceEdit->keySequence());
    }

    {
        using namespace Settings::Palette;

        set(Type::Color0, _ui->color0->palette().color(QPalette::Button));
        set(Type::Color1, _ui->color1->palette().color(QPalette::Button));
        set(Type::Color2, _ui->color2->palette().color(QPalette::Button));
        set(Type::Color3, _ui->color3->palette().color(QPalette::Button));
    }

    QDialog::accept();
}

void Preference::reject()
{
    QDialog::reject();
}

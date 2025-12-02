//
// Created by plouvel on 11/29/25.
//

#ifndef GBEMU_PREFERENCE_HXX
#define GBEMU_PREFERENCE_HXX

#include <QDialog>
#include <QKeySequenceEdit>

#include "Emulator.hxx"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Preference;
}
QT_END_NAMESPACE

class Preference final : public QDialog
{
    Q_OBJECT

  public:
    explicit Preference(QWidget* parent = nullptr);
    ~Preference() override;

    void accept() override;
    void reject() override;

  private:
    QString         _bootRomPath;
    Ui::Preference* _ui;
};

#endif  // GBEMU_PREFERENCE_HXX

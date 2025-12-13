//
// Created by plouvel on 12/13/25.
//

#ifndef GBEMU_ADDRESSINPUTDIALOG_HXX
#define GBEMU_ADDRESSINPUTDIALOG_HXX

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class AddressInputDialog;
}
QT_END_NAMESPACE

class AddressInputDialog final : public QDialog
{
    Q_OBJECT

  public:
    explicit AddressInputDialog(const QString& title, int maxSize = 4, QWidget* parent = nullptr);
    ~AddressInputDialog() override;
    uint64_t getAddress() const;

  private:
    Ui::AddressInputDialog* ui;
};

#endif  // GBEMU_ADDRESSINPUTDIALOG_HXX

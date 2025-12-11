//
// Created by plouvel on 12/11/25.
//

#ifndef GBEMU_REGISTERMODEL_HXX
#define GBEMU_REGISTERMODEL_HXX
#include <qabstractitemmodel.h>

class RegisterModel final : public QAbstractTableModel
{
    Q_OBJECT
  public:
    struct RegisterEntry
    {
        enum class Size : uint8_t
        {
            Eight     = 8,
            Sixteen   = 16,
            ThirtyTwo = 32,
            SixtyFour = 64
        };

        QString  name;
        uint64_t value;
        Size     bitSize;
        bool     editable{true};
    };

    RegisterModel(std::initializer_list<RegisterEntry> registers = {}, QObject* parent = nullptr);

    [[nodiscard]] QVariant      headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int           rowCount(const QModelIndex& parent) const override;
    [[nodiscard]] int           columnCount(const QModelIndex& parent) const override;
    [[nodiscard]] QVariant      data(const QModelIndex& index, int role) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]] bool          setData(const QModelIndex& index, const QVariant& value, int role) override;

  signals:
    void registerChanged(const RegisterModel::RegisterEntry& entry);

  private:
    std::vector<RegisterEntry> _registers;
};

#endif  // GBEMU_REGISTERMODEL_HXX

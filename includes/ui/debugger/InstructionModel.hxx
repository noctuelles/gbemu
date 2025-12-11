//
// Created by plouvel on 12/11/25.
//

#ifndef GBEMU_INSTRUCTIONMODEL_HXX
#define GBEMU_INSTRUCTIONMODEL_HXX

#include "RegisterModel.hxx"

class InstructionModel final : public QAbstractListModel
{
    Q_OBJECT
  public:
    struct Instruction
    {
        QString          instruction{"ret"};
        uint16_t         address{};
        QVector<uint8_t> bytes{};
        bool             hasBreakpointDefined{};
    };

    explicit InstructionModel(QObject* parent = nullptr);

    [[nodiscard]] int           columnCount(const QModelIndex& parent) const override;
    [[nodiscard]] int           rowCount(const QModelIndex& parent) const override;
    [[nodiscard]] QVariant      data(const QModelIndex& index, int role) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]] bool          setData(const QModelIndex& index, const QVariant& value, int role) override;

  private:
    std::array<Instruction, 0x10000> _instructions;
};

#endif  // GBEMU_INSTRUCTIONMODEL_HXX

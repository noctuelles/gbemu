//
// Created by plouvel on 12/11/25.
//

#ifndef GBEMU_INSTRUCTIONMODEL_HXX
#define GBEMU_INSTRUCTIONMODEL_HXX

#include "Common.hxx"
#include "RegisterModel.hxx"
#include "hardware/core/SM83.hxx"

class InstructionModel final : public QAbstractListModel
{
    Q_OBJECT
  public:
    enum Role
    {
        BreakpointRole = Qt::UserRole + 1
    };

    explicit InstructionModel(QObject* parent = nullptr);

    void                      updateInstructions(const AddressSpace& addressSpace);
    [[nodiscard]] QModelIndex indexForAddress(uint16_t address) const;

    [[nodiscard]] int           columnCount(const QModelIndex& parent) const override;
    [[nodiscard]] int           rowCount(const QModelIndex& parent) const override;
    [[nodiscard]] QVariant      data(const QModelIndex& index, int role) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]] bool          setData(const QModelIndex& index, const QVariant& value, int role) override;

  private:
    SM83::Disassembler::DisassembledInstructions _instructions;
    std::unordered_map<uint16_t, bool>           _breakpoints;
};

#endif  // GBEMU_INSTRUCTIONMODEL_HXX

//
// Created by plouvel on 12/11/25.
//

#ifndef GBEMU_INSTRUCTIONSLISTVIEW_HXX
#define GBEMU_INSTRUCTIONSLISTVIEW_HXX

#include <QListView>

class InstructionListView final : public QListView
{
    Q_OBJECT
  public:
    explicit InstructionListView(QWidget* parent = nullptr);

    QModelIndex hoveredIndex() const;
    int         hoveredX() const;

  protected:
    void mouseMoveEvent(QMouseEvent* ev) override;
    void leaveEvent(QEvent* ev) override;

  private:
    QModelIndex _hoveredIndex;
    int         _hoverX{-1};
};

#endif  // GBEMU_INSTRUCTIONSLISTVIEW_HXX

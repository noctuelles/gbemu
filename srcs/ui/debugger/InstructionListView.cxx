//
// Created by plouvel on 12/11/25.
//

#include "ui/debugger/InstructionListView.hxx"

#include <QEvent>
#include <QInputDialog>
#include <QMenu>
#include <QMouseEvent>

#include "../../../includes/ui/debugger/utils/AddressInputDialog.hxx"
#include "ui/debugger/BreakpointDelegate.hxx"

InstructionListView::InstructionListView(QWidget* parent) : QListView(parent)
{
    setMouseTracking(true);
}

QModelIndex InstructionListView::hoveredIndex() const
{
    return _hoveredIndex;
}

int InstructionListView::hoveredX() const
{
    return _hoverX;
}

void InstructionListView::mouseMoveEvent(QMouseEvent* ev)
{
    _hoveredIndex = indexAt(ev->pos());
    _hoverX       = ev->pos().x();
    viewport()->update();
    QListView::mouseMoveEvent(ev);
}

void InstructionListView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        QMenu menu{};

        const auto* gotoAction{menu.addAction(tr("Goto address..."))};

        connect(gotoAction, &QAction::triggered, this,
                [this]
                {
                    AddressInputDialog dialog{tr("Goto address")};

                    if (const auto result{dialog.exec()}; result == QDialog::Accepted)
                    {
                        emit gotoAddress(dialog.getAddress());
                    }
                });

        menu.exec(event->globalPosition().toPoint());
    }

    QListView::mousePressEvent(event);
}

void InstructionListView::leaveEvent(QEvent* ev)
{
    _hoveredIndex = QModelIndex{};
    viewport()->update();
    QListView::leaveEvent(ev);
}

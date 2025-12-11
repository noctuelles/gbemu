//
// Created by plouvel on 12/11/25.
//

#include "ui/debugger/InstructionListView.hxx"

#include <QEvent>
#include <QMouseEvent>

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

void InstructionListView::leaveEvent(QEvent* ev)
{
    _hoveredIndex = QModelIndex{};
    viewport()->update();
    QListView::leaveEvent(ev);
}

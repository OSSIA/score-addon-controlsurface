#include "View.hpp"

#include <Process/Style/ScenarioStyle.hpp>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <State/MessageListSerialization.hpp>
#include <wobjectimpl.h>
W_OBJECT_IMPL(ControlSurface::View)
namespace ControlSurface
{

View::View(QGraphicsItem* parent) : LayerView{parent}
{
}

View::~View()
{
}

void View::paint_impl(QPainter* painter) const
{
}
}


void ControlSurface::View::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
}

void ControlSurface::View::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
}

void ControlSurface::View::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
}

void ControlSurface::View::dropEvent(QGraphicsSceneDragDropEvent* event)
{
  Mime<State::MessageList>::Deserializer des(*event->mimeData());
  auto list = des.deserialize();
  if (!list.isEmpty())
    addressesDropped(list);
}

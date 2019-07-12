
#include <score/command/Dispatchers/CommandDispatcher.hpp>

#include <ControlSurface/Presenter.hpp>
#include <ControlSurface/Process.hpp>
#include <ControlSurface/View.hpp>
#include <Process/Dataflow/Port.hpp>
#include <score/tools/IdentifierGeneration.hpp>
#include <score/model/path/PathSerialization.hpp>
#include <ControlSurface/CommandFactory.hpp>
#include <score/command/Dispatchers/MacroCommandDispatcher.hpp>
#include <Control/DefaultEffectItem.hpp>
#include <ControlSurface/Commands.hpp>

namespace ControlSurface
{

Presenter::Presenter(
    const Model& layer, View* view,
    const Process::ProcessPresenterContext& ctx, QObject* parent)
    : Process::LayerPresenter{ctx, parent}, m_model{layer}, m_view{view}
{
  connect(view, &View::addressesDropped,
          this, [this] (State::MessageList lst) {
    if(lst.isEmpty())
      return;

    MacroCommandDispatcher<AddControlMacro> disp{this->context().context.commandStack};
    for(auto& message : lst) {
      disp.submit(new AddControl{m_model, std::move(message)});
      qDebug() << "added " << message;
    }
    disp.commit();
  });


  auto item = new Media::Effect::DefaultEffectItem{m_model, this->context().context, m_view};
  item->setPos(50, 50);
}

void Presenter::setWidth(qreal val, qreal defaultWidth)
{
  m_view->setWidth(val);
}

void Presenter::setHeight(qreal val)
{
  m_view->setHeight(val);
}

void Presenter::putToFront()
{
  m_view->setOpacity(1);
}

void Presenter::putBehind()
{
  m_view->setOpacity(0.2);
}

void Presenter::on_zoomRatioChanged(ZoomRatio)
{
}

void Presenter::parentGeometryChanged()
{
}

const Process::ProcessModel& Presenter::model() const
{
  return m_model;
}

const Id<Process::ProcessModel>& Presenter::modelId() const
{
  return m_model.id();
}
}

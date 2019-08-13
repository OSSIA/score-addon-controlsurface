#pragma once
#include <score/command/AggregateCommand.hpp>
#include <ControlSurface/Process.hpp>
#include <Process/Dataflow/Port.hpp>
#include <score/tools/IdentifierGeneration.hpp>
#include <score/model/path/PathSerialization.hpp>
#include <ControlSurface/CommandFactory.hpp>
#include <Explorer/Explorer/DeviceExplorerModel.hpp>
namespace ControlSurface
{

class AddControlMacro final : public score::AggregateCommand
{
  SCORE_COMMAND_DECL(
      CommandFactoryName(),
      AddControlMacro,
      "Add controls")
};

class AddControl : public score::Command {
  SCORE_COMMAND_DECL(
      CommandFactoryName(),
      AddControl,
      "Add a control")
public:
  AddControl(const score::DocumentContext& ctx, const Model& proc, const State::Message& p)
    : m_model{proc}
    , m_id{getStrongId(proc.inlets())}
    , m_addr{Explorer::makeFullAddressAccessorSettings(p.address, ctx, 0., 1.)}
  {
    m_addr.value = p.value;
  }

  void undo(const score::DocumentContext& ctx) const override
  {
    auto& proc = m_model.find(ctx);
    proc.removeControl(m_id);
  }

  void redo(const score::DocumentContext& ctx) const override
  {
    auto& proc = m_model.find(ctx);
    proc.addControl(m_id, m_addr);
  }

private:
  void serializeImpl(DataStreamInput& s) const override
  {
    s << m_model << m_id << m_addr;
  }

  void deserializeImpl(DataStreamOutput& s) override
  {
    s >> m_model >> m_id >> m_addr;
  }

  Path<Model> m_model;
  Id<Process::Port> m_id;
  Device::FullAddressAccessorSettings m_addr;
};

}

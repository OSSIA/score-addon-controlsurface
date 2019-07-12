#pragma once
#include <score/command/AggregateCommand.hpp>
#include <ControlSurface/Process.hpp>
#include <Process/Dataflow/Port.hpp>
#include <score/tools/IdentifierGeneration.hpp>
#include <score/model/path/PathSerialization.hpp>
#include <ControlSurface/CommandFactory.hpp>

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
  AddControl(const Model& proc, const State::Message& p)
    : m_model{proc}
    , m_id{getStrongId(proc.outlets())}
    , m_msg{p}
  {
  }

  void undo(const score::DocumentContext& ctx) const override
  {
    auto& proc = m_model.find(ctx);
    auto it = ossia::find_if(proc.outlets(), [this] (const auto& outlet) { return outlet->id() == m_id; });
    SCORE_ASSERT(it != proc.outlets().end());
    proc.controlOutletRemoved(**it);
    proc.outlets().erase(it);
  }

  void redo(const score::DocumentContext& ctx) const override
  {
    auto& proc = m_model.find(ctx);
    auto ctl = new Process::ControlOutlet(m_id, &proc);
    ctl->setAddress(m_msg.address);
    ctl->setCustomData(m_msg.address.toString());
    ctl->setValue(m_msg.value);
    // TODO domain
    proc.outlets().push_back(std::move(ctl));
    proc.controlOutletAdded(m_id);
  }

private:
  void serializeImpl(DataStreamInput& s) const override
  {
    s << m_model << m_id << m_msg;
  }

  void deserializeImpl(DataStreamOutput& s) override
  {
    s >> m_model >> m_id >> m_msg;
  }

  Path<Model> m_model;
  Id<Process::Port> m_id;
  State::Message m_msg;
};

}

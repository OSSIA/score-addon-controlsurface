#pragma once
#include <Process/Execution/ProcessComponent.hpp>
#include <ControlSurface/Process.hpp>
#include <ossia/dataflow/node_process.hpp>
#include <ossia/dataflow/safe_nodes/executor.hpp>
#include <Process/Dataflow/Port.hpp>


namespace ossia
{
class control_surface_node : public ossia::graph_node
{
public:
  std::vector<ossia::value*> controls;
  ossia::value& add_control()
  {
    auto port = new ossia::outlet(ossia::value_port{});
    controls.push_back(new ossia::value);
    m_outlets.push_back(port);
    return *controls.back();
  }

  struct control_updater
  {
    ossia::value& control;
    ossia::value v;

    void operator()() noexcept
    {
      control = std::move(v);
    }
  };

  void run(token_request, exec_state_facade) noexcept override
  {
    int n = controls.size();
    for(int i = 0; i < n; i++)
    {
      m_outlets[i]->data.target<ossia::value_port>()->write_value(std::move(*controls[i]), 0);
    }
  }

  std::string label() const noexcept override
  {
    return "control surface";
  }
};

}
namespace ControlSurface
{
class Model;


  struct con_unvalidated
  {
    const Execution::Context& ctx;
    const int i;
    std::weak_ptr<ossia::control_surface_node> weak_node;
    void operator()(const ossia::value& val)
    {
      if (auto node = weak_node.lock())
      {
        ctx.executionQueue.enqueue(ossia::control_surface_node::control_updater{*node->controls[i], val});
      }
    }
  };


class ProcessExecutorComponent final
    : public Execution::ProcessComponent_T<ControlSurface::Model, ossia::node_process>
{
  COMPONENT_METADATA("bab572b1-37eb-4f32-8f72-d5b79b65cfe9")
public:
  ProcessExecutorComponent(
      ControlSurface::Model& element,
      const ::Execution::Context& ctx,
      const Id<score::Component>& id,
      QObject* parent)
      : Execution::
            ProcessComponent_T<ControlSurface::Model, ossia::node_process>{
                element,
                ctx,
                id,
                "ControlSurface",
                parent}
  {
    auto node = std::make_shared<ossia::control_surface_node>();
    this->node = node;
    this->m_ossia_process = std::make_shared<ossia::node_process>(this->node);


    // Initialize all the controls in the node with the current value.
    // And update the node when the UI changes

    int i = 0;
    for(auto& ctl : element.outlets())
    {
      ossia::value& p = node->add_control();
      auto ctrl = safe_cast<Process::ControlOutlet*>(ctl);
      p = ctrl->value(); // TODO does this make sense ?

      std::weak_ptr weak_node = node;
      QObject::connect(
            ctrl,
            &Process::ControlOutlet::valueChanged,
            this,
            con_unvalidated{ctx, i, weak_node});
      i++;
    }
  }

  ~ProcessExecutorComponent() {
  }
};
using ProcessExecutorComponentFactory
    = Execution::ProcessComponentFactory_T<ProcessExecutorComponent>;
}

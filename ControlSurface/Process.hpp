#pragma once
#include <Process/GenericProcessFactory.hpp>
#include <Process/Process.hpp>

#include <ControlSurface/Metadata.hpp>

namespace ControlSurface
{
class Model final : public Process::ProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(ControlSurface::Model)
  W_OBJECT(Model)

public:
  Model(
      const TimeVal& duration, const Id<Process::ProcessModel>& id,
      QObject* parent);

  template <typename Impl>
  Model(Impl& vis, QObject* parent) : Process::ProcessModel{vis, parent}
  {
    vis.writeTo(*this);
  }

  ~Model() override;

  using Process::ProcessModel::inlets;
  using Process::ProcessModel::outlets;

  Process::Inlets& inlets() noexcept { return m_inlets; }
  Process::Outlets& outlets() noexcept { return m_outlets; }

private:
  QString prettyName() const noexcept override;
  void startExecution() override;
  void stopExecution() override;
  void reset() override;

  void setDurationAndScale(const TimeVal& newDuration) noexcept override;
  void setDurationAndGrow(const TimeVal& newDuration) noexcept override;
  void setDurationAndShrink(const TimeVal& newDuration) noexcept override;
};

using ProcessFactory = Process::ProcessFactory_T<ControlSurface::Model>;
}

#include "Process.hpp"

#include <wobjectimpl.h>

W_OBJECT_IMPL(ControlSurface::Model)
namespace ControlSurface
{

Model::Model(
    const TimeVal& duration, const Id<Process::ProcessModel>& id,
    QObject* parent)
    : Process::ProcessModel{duration, id, "ControlSurfaceProcess", parent}
{
  metadata().setInstanceName(*this);
}

Model::~Model()
{
}

QString Model::prettyName() const noexcept
{
  return tr("Control surface");
}

void Model::startExecution()
{
}

void Model::stopExecution()
{
}

void Model::reset()
{
}

void Model::setDurationAndScale(const TimeVal& newDuration) noexcept
{
}

void Model::setDurationAndGrow(const TimeVal& newDuration) noexcept
{
}

void Model::setDurationAndShrink(const TimeVal& newDuration) noexcept
{
}
}
template <>
void DataStreamReader::read(const ControlSurface::Model& proc)
{
  insertDelimiter();
}

template <>
void DataStreamWriter::write(ControlSurface::Model& proc)
{
  checkDelimiter();
}

template <>
void JSONObjectReader::read(const ControlSurface::Model& proc)
{
}

template <>
void JSONObjectWriter::write(ControlSurface::Model& proc)
{
}

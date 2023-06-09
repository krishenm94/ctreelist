#include "DockSourceModelsUpdater.h"

#include "MockUpOptionsModel.h"

#include "OptionsService.h"

void DockSourceModelsUpdater::populate(const QVector<Model&> &models) const
{
    OptionsService::populate(*m_optionsModel, models);
}

void DockSourceModelsUpdater::add(Model &model) const
{
    OptionsService::add(*m_optionsModel, model);
}

void DockSourceModelsUpdater::remove(Model &model) const
{
    OptionsService::remove(*m_optionsModel, model);
}

void DockSourceModelsUpdater::clear() const
{
    OptionsService::clear(*m_optionsModel);
}

DockSourceModelsUpdater::DockSourceModelsUpdater():
    m_optionsModel{std::make_unique<MockUpOptionsModel>()}
{}

MockUpOptionsModel& DockSourceModelsUpdater::optionsModel() const
{
    return *m_optionsModel;
}

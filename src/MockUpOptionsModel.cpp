#include "MockUpOptionsModel.h"

#include "GroupNode.h"

QVector<Model&> MockUpOptionsModel::sourceModels() const
{
    const auto lock = lockForRead();
    return m_sourceModels;
}

void MockUpOptionsModel::setSourceModels(QVector<Model&> models)
{
    set(&m_sourceModels,
        &models,
        [this]()
        {
            emit sourceModelsChanged();
        });
}

std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> MockUpOptionsModel::availableOptions() const
{
    const auto lock = lockForRead();
    return m_availableOptions;
}

void MockUpOptionsModel::setAvailableOptions(
    std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> availableOptions)
{
    set(&m_availableOptions,
        &availableOptions,
        [this]()
        {
            emit availableChanged();
        });
}

std::map<size_t, std::string> MockUpOptionsModel::selectedGroupOptions() const
{
    const auto lock = lockForRead();
    return m_selectedGroupOptions;
}

void MockUpOptionsModel::setSelectedGroupOptions(std::map<size_t, std::string> selectedOptions)
{
    set(&m_selectedGroupOptions,
        &selectedOptions,
        [this]()
        {
            emit selectedChanged();
        });
}

std::optional<std::string> MockUpOptionsModel::selectedSortOption() const
{
    const auto lock = lockForRead();
    return m_selectedSortOption;
}

void MockUpOptionsModel::setSelectedSortOption(std::optional<std::string> selectedOptions)
{
    set(&m_selectedSortOption,
        &selectedOptions,
        [this]()
        {
            emit selectedChanged();
        });
}

void MockUpOptionsModel::setRoot(GroupNode *node)
{
    {
        const auto lock = lockForWrite();
        if (node == m_root)
        {
            return;
        }

        if (m_root != nullptr)
        {
            m_root->deleteLater();
        }
    }

    set(&m_root,
        &node,
        [this]()
        {
            emit rootChanged();
        });
}

GroupNode* MockUpOptionsModel::root() const
{
    const auto lock = lockForRead();
    return m_root;
}

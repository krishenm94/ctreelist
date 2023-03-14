#pragma once

#include "Helper.h"

#include "Model.h"

class MetaModel;

struct SelectedOption
{
    const std::string key;
    const size_t index;
};

// TODO: KM: Consider splitting into GroupTreeModel and HeaderModel
class MockUpOptionsModel final : public Model
{
Q_OBJECT

public:
    CTOR_DEFAULT(MockUpOptionsModel)

    [[nodiscard]] QVector<Model&> sourceModels() const;
    void setSourceModels(QVector<Model&> models);

    [[nodiscard]] std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> availableOptions() const;
    void setAvailableOptions(
        std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> availableOptions
    );

    [[nodiscard]] std::map<size_t, std::string> selectedGroupOptions() const;
    void setSelectedGroupOptions(std::map<size_t, std::string> selectedOptions);

    [[nodiscard]] std::optional<std::string> selectedSortOption() const;
    void setSelectedSortOption(std::optional<std::string> selectedOptions);

    void setRoot(GroupNode *node);
    [[nodiscard]] GroupNode* root() const;

private:
    QVector<Model&> m_sourceModels;
    std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> m_availableOptions;
    std::map<size_t, std::string> m_selectedGroupOptions;
    std::optional<std::string> m_selectedSortOption;

    GroupNode *m_root;

signals:
    void availableChanged() const;
    void selectedChanged() const;
    void sourceModelsChanged() const;
    void rootChanged() const;
};

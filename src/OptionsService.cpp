#include "OptionsService.h"

#include "MockUpOptionsModel.h"
#include "OrderModel.h"

#include "GroupNode.h"

#include <gsl/gsl_util>

void OptionsService::populate(MockUpOptionsModel &optionsModel,
                              const std::vector<Model&> &sourceModels)
{
    optionsModel.setSourceModels(sourceModels);

    auto propertyMaps = std::vector<std::map<const std::string, std::shared_ptr<PropertyModel>>>{};
    propertyMaps.reserve(sourceModels.size());

    for (const auto &sourceModel : sourceModels)
    {
        propertyMaps.emplace_back(sourceModel->propertyMap());
    }

    auto keyGroupedProperties = std::map<const std::string, Properties>();
    auto keyCounts = std::map<std::string, size_t>{};
    for (const auto &map : propertyMaps)
    {
        for (const auto &[key, property] : map)
        {
            if (keyGroupedProperties.contains(key))
            {
                auto properties = keyGroupedProperties[key];
                properties.emplace_back(property);
                keyGroupedProperties[key] = properties;
            }
            else
            {
                keyGroupedProperties[key] = {property};
            }

            keyCounts[key] = keyCounts.contains(key) ? ++keyCounts[key] : 1;
        }
    }

    auto commonKeyPropertiesMap = std::map<const std::string, Properties>();
    for (const auto &[key, count] : keyCounts)
    {
        if (count == sourceModels.size())
        {
            commonKeyPropertiesMap[key] = std::move(keyGroupedProperties[key]);
        }
    }

    optionsModel.setAvailableOptions(commonKeyPropertiesMap);
}

std::vector<std::shared_ptr<Model>> OptionsService::convertOrdersToModels(
    std::vector<std::shared_ptr<OrderModel>> orders)
{
    std::vector<std::shared_ptr<Model>> sourceModels;
    sourceModels.reserve(orders.size());

    for (const auto &order : orders)
    {
        sourceModels.emplace_back(order);
    }

    return sourceModels;
}

void OptionsService::appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key)
{
    const auto selectedOptions = optionsModel.selectedGroupOptions();
    auto smallestFreeIndex = 0;
    for (const auto &[index, key] : selectedOptions)
    {
        if (smallestFreeIndex < index)
        {
            break;
        }

        ++smallestFreeIndex;
    }
    appendGroupSelectedOption(optionsModel, key, smallestFreeIndex);
}

void OptionsService::appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key, size_t index)
{
    auto selectedOptions = optionsModel.selectedGroupOptions();
    selectedOptions[index] = key;
    optionsModel.setSelectedGroupOptions(selectedOptions);
}

void OptionsService::removeGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string targetKey)
{
    auto selectedOptions = optionsModel.selectedGroupOptions();

    auto targetIndex = std::optional<size_t>();
    for (const auto &[index, key] : selectedOptions)
    {
        if (targetKey != key)
        {
            continue;
        }

        targetIndex = index;
        break;
    }

    ASSERT(
        targetIndex.has_value(),
        std::string("Selected group options does not contain key: " + targetKey).c_str()
    )

    const auto erasedCount = selectedOptions.erase(targetIndex.value());

    ASSERT(
        erasedCount < 2,
        QString("Selected group options must be unique. Grouping, Count: (%1, %2)")
        .arg(QString::fromStdString(targetKey)).arg(erasedCount)
        .toStdString().c_str()
    )

    optionsModel.setSelectedGroupOptions(selectedOptions);
}

void OptionsService::clearSelectedOptions(MockUpOptionsModel &optionsModel)
{
    optionsModel.setSelectedGroupOptions({});
    optionsModel.setSelectedSortOption(std::nullopt);
}

GroupNode* OptionsService::createGroupTree(MockUpOptionsModel &optionsModel)
{
    auto selectedOptions = optionsModel.selectedGroupOptions();

    auto orderedKeys = QVector<std::string>(gsl::narrow<int>(selectedOptions.size()));

    for (const auto &[index, key] : selectedOptions)
    {
        orderedKeys << key;
    }

    auto *const root = GroupNode::createRoot(optionsModel, optionsModel.sourceModels());
    createNodes(optionsModel, root, orderedKeys, optionsModel.sourceModels());

    return root;
}

void OptionsService::search(MockUpOptionsModel &optionsModel, const QString &text)
{
    // TODO: new_list: complete
}

void OptionsService::sort(
    const MockUpOptionsModel &optionsModel,
    std::vector<std::shared_ptr<Model>> &unsortedModels
)
{
    const auto selectedSortOption = optionsModel.selectedSortOption();

    if (!selectedSortOption.has_value())
    {
        return;
    }

    std::ranges::sort(unsortedModels.begin(),
                      unsortedModels.end(),
                      [&selectedSortOption](auto left, auto right)
                      {
                          return left->getPropertyModel(selectedSortOption.value()) <
                                 right->getPropertyModel(selectedSortOption.value());
                      });
}

std::map<QString, std::vector<std::shared_ptr<Model>>> OptionsService::generateValueModelsMap(
    const std::vector<std::shared_ptr<Model>> &sourceModels,
    const std::string &key)
{
    auto valueModelsMap = std::map<QString, std::vector<std::shared_ptr<Model>>>{};
    for (const auto &sourceModel : sourceModels)
    {
        const auto value = sourceModel->getPropertyModel(key)->getDisplayString();
        if (valueModelsMap.contains(value))
        {
            auto valueModels = valueModelsMap[value];
            valueModels.emplace_back(sourceModel);
            valueModelsMap[value] = valueModels;
        }
        else
        {
            valueModelsMap[value] = {sourceModel};
        }
    }

    return valueModelsMap;
}

void OptionsService::createNodes(
    const MockUpOptionsModel &optionsModel,
    GroupNode *parent,
    QVector<std::string> keys,
    const std::vector<std::shared_ptr<Model>> &sourceModels
)
{
    if (keys.empty())
    {
        return;
    }

    const auto currentKey = keys.first();
    keys.pop_front();
    const auto valueModelsMap = generateValueModelsMap(sourceModels, currentKey);
    for (const auto &[value, _sourceModels] : valueModelsMap)
    {
        auto sourceModelsCopy = _sourceModels;
        if (keys.empty())
        {
            sort(optionsModel, sourceModelsCopy);
        }
        auto *const node = new GroupNode(parent, currentKey, value, sourceModelsCopy);
        createNodes(optionsModel, node, keys, sourceModelsCopy);
    }
}

#include "MockUpDockHeaderView.h"

#include "Icons.h"

#include "MockUpOptionsModel.h"
#include "PropertyModel.h"

#include "OptionsService.h"

#include "MenuTitle.h"
#include "SearchBox.h"
#include "ToolButton.h"

#include <QHBoxLayout>
#include <QMenu>

// TODO: new_list: Can be further decomposed into separate widgets

MockUpDockHeaderView::MockUpDockHeaderView(MockUpOptionsModel &optionsModel):
    m_optionsModel{optionsModel},
    m_sortButton{new SmallDarkToolButton(ICON_SORT)},
    m_groupButton{new SmallDarkToolButton(ICON_RECTANGLE_SELECT)},
    m_searchBox{new SearchBox()}
{
    auto *const layout = new QHBoxLayout();
    layout->setContentsMargins(QMargins());
    layout->addWidget(m_sortButton);
    layout->addWidget(m_groupButton);
    layout->addWidget(m_searchBox);
    setLayout(layout);

    connect(&m_optionsModel, &MockUpOptionsModel::availableChanged, &MockUpDockHeaderView::onAvailableOptionsChanged);
    connect(m_searchBox, &SearchBox::textChanged, &MockUpDockHeaderView::onSearchBoxChanged);
}

void MockUpDockHeaderView::populateGroupMenu() const
{
    // TODO: new_list: Reduce code duplication with `populateSortMenu`
    auto *const menu = new QMenu(m_groupButton);
    menu->addAction(new MenuTitle(tr("Group By"), menu));

    for (const auto &[key, properties] : m_optionsModel.availableOptions())
    {
        ASSERT_FATAL(!properties.empty(),
                     std::string("Properties for given key should not be empty. Key: " + key).c_str())

        const auto firstProperty = properties[0];
        auto *const action = new QAction(firstProperty->name(), menu);
        action->setToolTip(firstProperty->description().value_or(""));
        action->setWhatsThis(key.c_str());
        action->setCheckable(true);

        connect(action,
                &QAction::triggered,
                [this, key](const bool enabled)
                {
                    enabled
                        ? OptionsService::appendGroupSelectedOption(m_optionsModel, key)
                        : OptionsService::removeGroupSelectedOption(m_optionsModel, key);
                });

        menu->addAction(action);
    }

    m_groupButton->setPopupMode(QToolButton::InstantPopup);
    m_groupButton->setMenu(menu);
}

void MockUpDockHeaderView::populateSortMenu() const
{
    auto *const menu = new QMenu(m_sortButton);
    auto *const actionGroup = new QActionGroup(menu);
    menu->addAction(new MenuTitle(tr("Group By"), menu));

    for (const auto &[key, properties] : m_optionsModel.availableOptions())
    {
        ASSERT_FATAL(!properties.empty(),
                     std::string("Properties for given key should not be empty. Key: " + key).c_str())

        const auto firstProperty = properties[0];
        auto *const action = new QAction(firstProperty->name(), menu);
        action->setToolTip(firstProperty->description().value_or(""));
        action->setWhatsThis(key.c_str());
        action->setCheckable(true);

        connect(action,
                &QAction::triggered,
                [this, key](const bool enabled)
                {
                    m_optionsModel.setSelectedSortOption(enabled ? std::make_optional(key) : std::nullopt);
                });

        actionGroup->addAction(action);
        menu->addAction(action);
    }

    m_sortButton->setPopupMode(QToolButton::InstantPopup);
    m_sortButton->setMenu(menu);
}

void MockUpDockHeaderView::onSearchBoxChanged(const QString &text) const
{
    OptionsService::search(m_optionsModel, text);
}

void MockUpDockHeaderView::onAvailableOptionsChanged() const
{
    populateGroupMenu();
    populateSortMenu();
}

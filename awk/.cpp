diff --git a/sources/libraries/applications/dtectx/main/MainWindow.cpp b/sources/libraries/applications/dtectx/main/MainWindow.cpp
index e5c9ac35350f138869f8141089acc526832041b6..689e304bef969c3911485bf07257e657e27762fa 100644
--- a/sources/libraries/applications/dtectx/main/MainWindow.cpp
+++ b/sources/libraries/applications/dtectx/main/MainWindow.cpp
@@ -50,6 +50,7 @@
 #include "MainToolbarView.h"
 #include "Menu.h"
 #include "MenuSection.h"
+#include "MockUpDockView.h"
 #include "ModelStore.h"
 #include "NotificationsDockView.h"
 #include "NotificationsToolbarView.h"
@@ -498,17 +499,29 @@ void MainWindow::createDocks()
                                                                         ads::LeftDockWidgetArea);
 
     // TOP-RIGHT AREA
+    auto *const topRightDockArea = DocksViewService::createPermanentDock(
+        m_mockUpDockAction,
+        PermanentDock::MockUp,
+        tr("Mock Up"),
+        SvgIcon(ICON_PENCIL_2),
+        new MockUpDockView(),
+        dockManager(),
+        nullptr,
+        ads::CDockWidget::eInsertMode::ForceNoScrollArea,
+        ads::RightDockWidgetArea
+    );
+
     // Toolbox
-    auto *const topRightDockArea = DocksViewService::createPermanentDock(m_toolboxDockAction,
-                                                                         PermanentDock::Toolbox,
-                                                                         tr("Toolbox"),
-                                                                         SvgIcon(ICON_HAMMER),
-                                                                         new ToolboxDockView(this),
-                                                                         dockManager(),
-                                                                         nullptr,
-                                                                         ads::CDockWidget::eInsertMode::
-                                                                         ForceNoScrollArea,
-                                                                         ads::RightDockWidgetArea);
+    DocksViewService::createPermanentDock(
+        m_toolboxDockAction,
+        PermanentDock::Toolbox,
+        tr("Toolbox"),
+        SvgIcon(ICON_HAMMER),
+        new ToolboxDockView(this),
+        dockManager(),
+        topRightDockArea,
+        ads::CDockWidget::eInsertMode::ForceNoScrollArea
+    );
 
     // History
     DocksViewService::createPermanentDock(m_historyDockAction,
@@ -646,6 +659,10 @@ void MainWindow::createViewMenu()
     m_ordersDockAction->setCheckable(true);
     m_viewMenu->addAction(m_ordersDockAction, Menu::ActionOption::DontCloseOnClick);
 
+    m_mockUpDockAction = new QAction(tr("Mock Up"), this);
+    m_mockUpDockAction->setCheckable(true);
+    m_viewMenu->addAction(m_mockUpDockAction, Menu::ActionOption::DontCloseOnClick);
+
     m_devicesDockAction = new QAction(tr("Devices"), this);
     m_devicesDockAction->setCheckable(true);
     m_viewMenu->addAction(m_devicesDockAction, Menu::ActionOption::DontCloseOnClick);
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..3eb256260d386917398d1bb21fd66ad4432eebee
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.cpp
@@ -0,0 +1,34 @@
+#include "DockSourceModelsUpdater.h"
+
+#include "MockUpOptionsModel.h"
+
+#include "OptionsService.h"
+
+void DockSourceModelsUpdater::populate(const QVector<Model&> &models) const
+{
+    OptionsService::populate(*m_optionsModel, models);
+}
+
+void DockSourceModelsUpdater::add(Model &model) const
+{
+    OptionsService::add(*m_optionsModel, model);
+}
+
+void DockSourceModelsUpdater::remove(Model &model) const
+{
+    OptionsService::remove(*m_optionsModel, model);
+}
+
+void DockSourceModelsUpdater::clear() const
+{
+    OptionsService::clear(*m_optionsModel);
+}
+
+DockSourceModelsUpdater::DockSourceModelsUpdater():
+    m_optionsModel{std::make_unique<MockUpOptionsModel>()}
+{}
+
+MockUpOptionsModel& DockSourceModelsUpdater::optionsModel() const
+{
+    return *m_optionsModel;
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNode.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNode.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..3586167f3a35d73f58d6b2c35265eb59adef1566
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNode.cpp
@@ -0,0 +1,105 @@
+#include "GroupNode.h"
+
+#include "MockUpOptionsModel.h"
+
+GroupNode::GroupNode(
+    GroupNode *parent,
+    const std::string &key,
+    const QString &value,
+    const std::vector<std::shared_ptr<MetaModel>> &metaModels
+) :
+    parent{parent},
+    key{key},
+    value{value},
+    row{parent->row + 1},
+    metaModels{metaModels}
+{
+    parent->m_children.push_back(this);
+    setParent(parent);
+}
+
+GroupNode* GroupNode::createRoot(MockUpOptionsModel &parent, const std::vector<std::shared_ptr<MetaModel>> &metaModels)
+{
+    return new GroupNode(parent, metaModels);
+}
+
+GroupNode::GroupNode(MockUpOptionsModel &parent, const std::vector<std::shared_ptr<MetaModel>> &metaModels) :
+    parent{nullptr},
+    value(""),
+    row{0},
+    metaModels{metaModels}
+{
+    parent.setRoot(this);
+    setParent(&parent);
+}
+
+bool GroupNode::isRoot() const
+{
+    ASSERT(!((parent == nullptr) ^ (row == 0)), "If node has no parent, then it's row must be zero and vice versa.")
+    return row == 0 && parent == nullptr;
+}
+
+bool GroupNode::isTerminal() const
+{
+    return m_children.empty();
+}
+
+bool GroupNode::visibility() const
+{
+    return m_visibility;
+}
+
+void GroupNode::setVisibility(bool value)
+{
+    if (m_visibility == value)
+    {
+        return;
+    }
+
+    m_visibility = value;
+
+    emit visibilityChanged(value);
+    emit dataChanged();
+}
+
+bool GroupNode::selected() const
+{
+    return m_selected;
+}
+
+void GroupNode::setSelected(bool value)
+{
+    if (m_selected == value)
+    {
+        return;
+    }
+
+    m_selected = value;
+
+    emit selectedChanged(value);
+    emit dataChanged();
+}
+
+bool GroupNode::expanded() const
+{
+    return m_expanded;
+}
+
+void GroupNode::setExpanded(bool value)
+{
+    if (m_expanded == value)
+    {
+        return;
+    }
+
+    m_expanded = value;
+
+    emit expandedChanged(value);
+    emit dataChanged();
+}
+
+QVector<GroupNode*> GroupNode::children() const
+{
+    const auto lock = lockForRead();
+    return m_children;
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNodeView.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNodeView.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..97d93d0a98427758ba6b143f8454ae47e94c35b1
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNodeView.cpp
@@ -0,0 +1,78 @@
+#include "GroupNodeView.h"
+
+#include "OrderModel.h"
+
+#include "ExpanderButton.h"
+#include "GroupNode.h"
+
+#include <QLabel>
+#include <QVBoxLayout>
+
+GroupNodeView::GroupNodeView(const MockUpOptionsModel &optionsModel, GroupNode *node) :
+    m_optionsModel{optionsModel},
+    m_node(node),
+    m_contentLayout{new QVBoxLayout()}
+{
+    setLayout(populate());
+}
+
+QLayout* GroupNodeView::populate() const
+{
+    auto *layout = new QVBoxLayout();
+    layout->setContentsMargins(QMargins{});
+
+    if (!m_node->isRoot())
+    {
+        layout->addWidget(createHeader());
+    }
+
+    m_contentLayout->setContentsMargins(QMargins{});
+
+    if (m_node->isTerminal())
+    {
+        for (const auto &metaModel : m_node->metaModels)
+        {
+            m_contentLayout->addWidget(createLeafView(*metaModel));
+        }
+    }
+    else
+    {
+        for (auto &child : m_node->children())
+        {
+            m_contentLayout->addWidget(new GroupNodeView(m_optionsModel, child));
+        }
+    }
+
+    auto *const wrapperLayout = new QHBoxLayout();
+    wrapperLayout->setContentsMargins(QMargins());
+
+    if (!m_node->isRoot())
+    {
+        wrapperLayout->addWidget(new QLabel("-"));
+    }
+
+    wrapperLayout->addLayout(m_contentLayout);
+    layout->addLayout(wrapperLayout);
+    return layout;
+}
+
+QWidget* GroupNodeView::createHeader() const
+{
+    // TODO: new_list: create factory
+    auto *const expander = new ExpanderButton(m_node->value);
+
+    connect(expander,
+            &QPushButton::released,
+            [this]()
+            {
+                m_node->setExpanded(m_node->expanded());
+            });
+
+    return expander;
+}
+
+QWidget* GroupNodeView::createLeafView(MetaModel &metaModel) const
+{
+    // TODO: new_list: create factory
+    return new QLabel(dynamic_cast<OrderModel&>(metaModel).name());
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpContentView.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpContentView.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..eb120f7d9e4df50571da7cf82bfe125a0a503730
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpContentView.cpp
@@ -0,0 +1,45 @@
+#include "MockUpContentView.h"
+
+#include "MockUpOptionsModel.h"
+
+#include "OptionsService.h"
+
+#include "Common.h"
+#include "GroupNodeView.h"
+#include "UiUtilities.h"
+
+#include <QScrollArea>
+#include <QVBoxLayout>
+
+MockUpContentView::MockUpContentView(MockUpOptionsModel &optionsModel):
+    m_optionsModel{optionsModel},
+    m_scrollAreaLayout{new QVBoxLayout()}
+{
+    auto *const scrollArea = Common::uiUtilities().wrapInScrollArea(m_scrollAreaLayout);
+
+    auto *const layout = new QVBoxLayout();
+    layout->setContentsMargins(QMargins());
+    layout->addWidget(scrollArea);
+
+    setLayout(layout);
+
+    connect(m_optionsModel,
+            &MockUpOptionsModel::selectedChanged,
+            [this]()
+            {
+                createContent();
+            });
+
+    createContent();
+}
+
+void MockUpContentView::createContent()
+{
+    auto *const root = OptionsService::createGroupTree(m_optionsModel);
+    auto *const rootView = new GroupNodeView(m_optionsModel, root);
+
+    m_scrollAreaLayout->setContentsMargins(QMargins());
+    clearLayout(m_scrollAreaLayout);
+    m_scrollAreaLayout->addWidget(rootView);
+    m_scrollAreaLayout->addStretch(1);
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockHeaderView.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockHeaderView.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..58d2885307e10b1b6ddc0d707dde33c8eeb27380
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockHeaderView.cpp
@@ -0,0 +1,110 @@
+#include "MockUpDockHeaderView.h"
+
+#include "Icons.h"
+
+#include "MockUpOptionsModel.h"
+#include "PropertyModel.h"
+
+#include "OptionsService.h"
+
+#include "MenuTitle.h"
+#include "SearchBox.h"
+#include "ToolButton.h"
+
+#include <QHBoxLayout>
+#include <QMenu>
+
+// TODO: new_list: Can be further decomposed into separate widgets
+
+MockUpDockHeaderView::MockUpDockHeaderView(MockUpOptionsModel &optionsModel):
+    m_optionsModel{optionsModel},
+    m_sortButton{new SmallDarkToolButton(ICON_SORT)},
+    m_groupButton{new SmallDarkToolButton(ICON_RECTANGLE_SELECT)},
+    m_searchBox{new SearchBox()}
+{
+    auto *const layout = new QHBoxLayout();
+    layout->setContentsMargins(QMargins());
+    layout->addWidget(m_sortButton);
+    layout->addWidget(m_groupButton);
+    layout->addWidget(m_searchBox);
+    setLayout(layout);
+
+    connect(&m_optionsModel, &MockUpOptionsModel::availableChanged, &MockUpDockHeaderView::onAvailableOptionsChanged);
+    connect(m_searchBox, &SearchBox::textChanged, &MockUpDockHeaderView::onSearchBoxChanged);
+}
+
+void MockUpDockHeaderView::populateGroupMenu() const
+{
+    // TODO: new_list: Reduce code duplication with `populateSortMenu`
+    auto *const menu = new QMenu(m_groupButton);
+    menu->addAction(new MenuTitle(tr("Group By"), menu));
+
+    for (const auto &[key, properties] : m_optionsModel.availableOptions())
+    {
+        ASSERT_FATAL(!properties.empty(),
+                     std::string("Properties for given key should not be empty. Key: " + key).c_str())
+
+        const auto firstProperty = properties[0];
+        auto *const action = new QAction(firstProperty->name(), menu);
+        action->setToolTip(firstProperty->description().value_or(""));
+        action->setWhatsThis(key.c_str());
+        action->setCheckable(true);
+
+        connect(action,
+                &QAction::triggered,
+                [this, key](const bool enabled)
+                {
+                    enabled
+                        ? OptionsService::appendGroupSelectedOption(m_optionsModel, key)
+                        : OptionsService::removeGroupSelectedOption(m_optionsModel, key);
+                });
+
+        menu->addAction(action);
+    }
+
+    m_groupButton->setPopupMode(QToolButton::InstantPopup);
+    m_groupButton->setMenu(menu);
+}
+
+void MockUpDockHeaderView::populateSortMenu() const
+{
+    auto *const menu = new QMenu(m_sortButton);
+    auto *const actionGroup = new QActionGroup(menu);
+    menu->addAction(new MenuTitle(tr("Group By"), menu));
+
+    for (const auto &[key, properties] : m_optionsModel.availableOptions())
+    {
+        ASSERT_FATAL(!properties.empty(),
+                     std::string("Properties for given key should not be empty. Key: " + key).c_str())
+
+        const auto firstProperty = properties[0];
+        auto *const action = new QAction(firstProperty->name(), menu);
+        action->setToolTip(firstProperty->description().value_or(""));
+        action->setWhatsThis(key.c_str());
+        action->setCheckable(true);
+
+        connect(action,
+                &QAction::triggered,
+                [this, key](const bool enabled)
+                {
+                    m_optionsModel.setSelectedSortOption(enabled ? std::make_optional(key) : std::nullopt);
+                });
+
+        actionGroup->addAction(action);
+        menu->addAction(action);
+    }
+
+    m_sortButton->setPopupMode(QToolButton::InstantPopup);
+    m_sortButton->setMenu(menu);
+}
+
+void MockUpDockHeaderView::onSearchBoxChanged(const QString &text) const
+{
+    OptionsService::search(m_optionsModel, text);
+}
+
+void MockUpDockHeaderView::onAvailableOptionsChanged() const
+{
+    populateGroupMenu();
+    populateSortMenu();
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockView.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockView.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..fea26a178746bb98cebfbb6198533782aaedcd7b
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockView.cpp
@@ -0,0 +1,20 @@
+#include "MockUpDockView.h"
+
+#include "DockSourceModelsUpdater.h"
+#include "MockUpContentView.h"
+#include "MockUpDockHeaderView.h"
+
+#include <QVBoxLayout>
+
+MockUpDockView::MockUpDockView(std::unique_ptr<DockSourceModelsUpdater> sourceUpdater, QWidget *parent):
+    DockWidgetBase{parent},
+    m_sourceUpdater{std::move(sourceUpdater)},
+    m_content{new MockUpContentView(sourceUpdater->optionsModel())},
+    m_header{new MockUpDockHeaderView(sourceUpdater->optionsModel())}
+{
+    auto *const layout = new QVBoxLayout();
+    layout->setContentsMargins(QMargins());
+    layout->addWidget(m_header);
+    layout->addWidget(m_content);
+    setLayout(layout);
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpOptionsModel.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpOptionsModel.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..15e5f6b009d2969ff35baea9a358bcfcae18edb6
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpOptionsModel.cpp
@@ -0,0 +1,97 @@
+#include "MockUpOptionsModel.h"
+
+#include "GroupNode.h"
+
+QVector<Model&> MockUpOptionsModel::sourceModels() const
+{
+    const auto lock = lockForRead();
+    return m_sourceModels;
+}
+
+void MockUpOptionsModel::setSourceModels(QVector<Model&> models)
+{
+    set(&m_sourceModels,
+        &models,
+        [this]()
+        {
+            emit sourceModelsChanged();
+        });
+}
+
+std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> MockUpOptionsModel::availableOptions() const
+{
+    const auto lock = lockForRead();
+    return m_availableOptions;
+}
+
+void MockUpOptionsModel::setAvailableOptions(
+    std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> availableOptions)
+{
+    set(&m_availableOptions,
+        &availableOptions,
+        [this]()
+        {
+            emit availableChanged();
+        });
+}
+
+std::map<size_t, std::string> MockUpOptionsModel::selectedGroupOptions() const
+{
+    const auto lock = lockForRead();
+    return m_selectedGroupOptions;
+}
+
+void MockUpOptionsModel::setSelectedGroupOptions(std::map<size_t, std::string> selectedOptions)
+{
+    set(&m_selectedGroupOptions,
+        &selectedOptions,
+        [this]()
+        {
+            emit selectedChanged();
+        });
+}
+
+std::optional<std::string> MockUpOptionsModel::selectedSortOption() const
+{
+    const auto lock = lockForRead();
+    return m_selectedSortOption;
+}
+
+void MockUpOptionsModel::setSelectedSortOption(std::optional<std::string> selectedOptions)
+{
+    set(&m_selectedSortOption,
+        &selectedOptions,
+        [this]()
+        {
+            emit selectedChanged();
+        });
+}
+
+void MockUpOptionsModel::setRoot(GroupNode *node)
+{
+    {
+        const auto lock = lockForWrite();
+        if (node == m_root)
+        {
+            return;
+        }
+
+        if (m_root != nullptr)
+        {
+            m_root->deleteLater();
+        }
+    }
+
+    set(&m_root,
+        &node,
+        [this]()
+        {
+            emit rootChanged();
+        });
+}
+
+GroupNode* MockUpOptionsModel::root() const
+{
+    const auto lock = lockForRead();
+    return m_root;
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..950e2a1bf841a373ba0ca9fc51d83c46209b7ad0
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.cpp
@@ -0,0 +1,223 @@
+#include "OptionsService.h"
+
+#include "MockUpOptionsModel.h"
+#include "OrderModel.h"
+
+#include "GroupNode.h"
+
+#include <gsl/gsl_util>
+
+void OptionsService::populate(MockUpOptionsModel &optionsModel,
+                              const std::vector<Model&> &sourceModels)
+{
+    optionsModel.setSourceModels(sourceModels);
+
+    auto propertyMaps = std::vector<std::map<const std::string, std::shared_ptr<PropertyModel>>>{};
+    propertyMaps.reserve(sourceModels.size());
+
+    for (const auto &sourceModel : sourceModels)
+    {
+        propertyMaps.emplace_back(sourceModel->propertyMap());
+    }
+
+    auto keyGroupedProperties = std::map<const std::string, Properties>();
+    auto keyCounts = std::map<std::string, size_t>{};
+    for (const auto &map : propertyMaps)
+    {
+        for (const auto &[key, property] : map)
+        {
+            if (keyGroupedProperties.contains(key))
+            {
+                auto properties = keyGroupedProperties[key];
+                properties.emplace_back(property);
+                keyGroupedProperties[key] = properties;
+            }
+            else
+            {
+                keyGroupedProperties[key] = {property};
+            }
+
+            keyCounts[key] = keyCounts.contains(key) ? ++keyCounts[key] : 1;
+        }
+    }
+
+    auto commonKeyPropertiesMap = std::map<const std::string, Properties>();
+    for (const auto &[key, count] : keyCounts)
+    {
+        if (count == sourceModels.size())
+        {
+            commonKeyPropertiesMap[key] = std::move(keyGroupedProperties[key]);
+        }
+    }
+
+    optionsModel.setAvailableOptions(commonKeyPropertiesMap);
+}
+
+std::vector<std::shared_ptr<Model>> OptionsService::convertOrdersToModels(
+    std::vector<std::shared_ptr<OrderModel>> orders)
+{
+    std::vector<std::shared_ptr<Model>> sourceModels;
+    sourceModels.reserve(orders.size());
+
+    for (const auto &order : orders)
+    {
+        sourceModels.emplace_back(order);
+    }
+
+    return sourceModels;
+}
+
+void OptionsService::appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key)
+{
+    const auto selectedOptions = optionsModel.selectedGroupOptions();
+    auto smallestFreeIndex = 0;
+    for (const auto &[index, key] : selectedOptions)
+    {
+        if (smallestFreeIndex < index)
+        {
+            break;
+        }
+
+        ++smallestFreeIndex;
+    }
+    appendGroupSelectedOption(optionsModel, key, smallestFreeIndex);
+}
+
+void OptionsService::appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key, size_t index)
+{
+    auto selectedOptions = optionsModel.selectedGroupOptions();
+    selectedOptions[index] = key;
+    optionsModel.setSelectedGroupOptions(selectedOptions);
+}
+
+void OptionsService::removeGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string targetKey)
+{
+    auto selectedOptions = optionsModel.selectedGroupOptions();
+
+    auto targetIndex = std::optional<size_t>();
+    for (const auto &[index, key] : selectedOptions)
+    {
+        if (targetKey != key)
+        {
+            continue;
+        }
+
+        targetIndex = index;
+        break;
+    }
+
+    ASSERT(
+        targetIndex.has_value(),
+        std::string("Selected group options does not contain key: " + targetKey).c_str()
+    )
+
+    const auto erasedCount = selectedOptions.erase(targetIndex.value());
+
+    ASSERT(
+        erasedCount < 2,
+        QString("Selected group options must be unique. Grouping, Count: (%1, %2)")
+        .arg(QString::fromStdString(targetKey)).arg(erasedCount)
+        .toStdString().c_str()
+    )
+
+    optionsModel.setSelectedGroupOptions(selectedOptions);
+}
+
+void OptionsService::clearSelectedOptions(MockUpOptionsModel &optionsModel)
+{
+    optionsModel.setSelectedGroupOptions({});
+    optionsModel.setSelectedSortOption(std::nullopt);
+}
+
+GroupNode* OptionsService::createGroupTree(MockUpOptionsModel &optionsModel)
+{
+    auto selectedOptions = optionsModel.selectedGroupOptions();
+
+    auto orderedKeys = QVector<std::string>(gsl::narrow<int>(selectedOptions.size()));
+
+    for (const auto &[index, key] : selectedOptions)
+    {
+        orderedKeys << key;
+    }
+
+    auto *const root = GroupNode::createRoot(optionsModel, optionsModel.sourceModels());
+    createNodes(optionsModel, root, orderedKeys, optionsModel.sourceModels());
+
+    return root;
+}
+
+void OptionsService::search(MockUpOptionsModel &optionsModel, const QString &text)
+{
+    // TODO: new_list: complete
+}
+
+void OptionsService::sort(
+    const MockUpOptionsModel &optionsModel,
+    std::vector<std::shared_ptr<Model>> &unsortedModels
+)
+{
+    const auto selectedSortOption = optionsModel.selectedSortOption();
+
+    if (!selectedSortOption.has_value())
+    {
+        return;
+    }
+
+    std::ranges::sort(unsortedModels.begin(),
+                      unsortedModels.end(),
+                      [&selectedSortOption](auto left, auto right)
+                      {
+                          return left->getPropertyModel(selectedSortOption.value()) <
+                                 right->getPropertyModel(selectedSortOption.value());
+                      });
+}
+
+std::map<QString, std::vector<std::shared_ptr<Model>>> OptionsService::generateValueModelsMap(
+    const std::vector<std::shared_ptr<Model>> &sourceModels,
+    const std::string &key)
+{
+    auto valueModelsMap = std::map<QString, std::vector<std::shared_ptr<Model>>>{};
+    for (const auto &sourceModel : sourceModels)
+    {
+        const auto value = sourceModel->getPropertyModel(key)->getDisplayString();
+        if (valueModelsMap.contains(value))
+        {
+            auto valueModels = valueModelsMap[value];
+            valueModels.emplace_back(sourceModel);
+            valueModelsMap[value] = valueModels;
+        }
+        else
+        {
+            valueModelsMap[value] = {sourceModel};
+        }
+    }
+
+    return valueModelsMap;
+}
+
+void OptionsService::createNodes(
+    const MockUpOptionsModel &optionsModel,
+    GroupNode *parent,
+    QVector<std::string> keys,
+    const std::vector<std::shared_ptr<Model>> &sourceModels
+)
+{
+    if (keys.empty())
+    {
+        return;
+    }
+
+    const auto currentKey = keys.first();
+    keys.pop_front();
+    const auto valueModelsMap = generateValueModelsMap(sourceModels, currentKey);
+    for (const auto &[value, _sourceModels] : valueModelsMap)
+    {
+        auto sourceModelsCopy = _sourceModels;
+        if (keys.empty())
+        {
+            sort(optionsModel, sourceModelsCopy);
+        }
+        auto *const node = new GroupNode(parent, currentKey, value, sourceModelsCopy);
+        createNodes(optionsModel, node, keys, sourceModelsCopy);
+    }
+}
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..ba351c7519a158480cf6ba2e885f03b33257b7aa
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.cpp
@@ -0,0 +1,40 @@
+#include "OrdersDockSourceUpdater.h"
+
+#include "ModelStore.h"
+#include "OrdersModel.h"
+
+OrdersDockSourceUpdater::OrdersDockSourceUpdater()
+{
+    const auto &orders = ModelStore::get<OrdersModel>();
+
+    populate(upcast(orders.orders()));
+
+    connect(orders,
+            &OrdersModel::added,
+            [this](const auto &order)
+            {
+                ASSERT(order != nullptr, "Order cannot be nullptr")
+                if (order != nullptr)
+                {
+                    add(*order);
+                }
+            });
+
+    connect(orders,
+            &OrdersModel::removed,
+            [this](const auto &order)
+            {
+                ASSERT(order != nullptr, "Order cannot be nullptr")
+                if (order != nullptr)
+                {
+                    remove(*order);
+                }
+            });
+
+    connect(orders,
+            &OrdersModel::cleared,
+            [this]()
+            {
+                clear();
+            });
+}

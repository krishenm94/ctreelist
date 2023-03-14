diff --git a/sources/libraries/applications/dtectx/main/MainWindow.h b/sources/libraries/applications/dtectx/main/MainWindow.h
index fe0c355a0273e27782dd5bba1e845d8e1df56451..78fe26bbdf65f3608d34e834fcb225fa4cc61f69 100644
--- a/sources/libraries/applications/dtectx/main/MainWindow.h
+++ b/sources/libraries/applications/dtectx/main/MainWindow.h
@@ -52,6 +52,7 @@ private:
 
     QAction *m_workspaceDockAction{nullptr};
     QAction *m_ordersDockAction{nullptr};
+    QAction *m_mockUpDockAction{nullptr};
     QAction *m_devicesDockAction{nullptr};
     QAction *m_notificationsDockAction{nullptr};
     QAction *m_ordersPropertiesDockAction{nullptr};
diff --git a/sources/libraries/framework/models/docks/DockName.h b/sources/libraries/framework/models/docks/DockName.h
index 15fea471202d7c9f93da76cec0977f7c6ea9effa..763f91955834562eacfed27481e849de9640e574 100644
--- a/sources/libraries/framework/models/docks/DockName.h
+++ b/sources/libraries/framework/models/docks/DockName.h
@@ -12,7 +12,8 @@ enum class PermanentDock
     BackgroundTasks,
     Notifications,
     OrdersProperties,
-    WorkspaceProperties
+    WorkspaceProperties,
+    MockUp
 };
 
 enum class RuntimeDock
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.h b/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.h
new file mode 100644
index 0000000000000000000000000000000000000000..f77a634db344c04846e97c6a60215b86b6ea1ff6
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.h
@@ -0,0 +1,57 @@
+#pragma once
+
+#include "Connector.h"
+
+#include <QObject>
+#include <QVector>
+
+class MockUpOptionsModel;
+class Model;
+
+class Dispatcher : public Connector<QObject>
+{
+Q_OBJECT
+
+protected:
+    CTOR_DEFAULT(Dispatcher)
+};
+
+class DockSourceModelsUpdater : public Dispatcher
+{
+Q_OBJECT
+
+    std::unique_ptr<MockUpOptionsModel> m_optionsModel;
+
+protected:
+    void populate(const QVector<Model&> &models) const;
+    void add(Model &model) const;
+    void remove(Model &model) const;
+    void clear() const;
+
+    template<typename TModel>
+    static QVector<Model&> upcast(std::vector<std::shared_ptr<TModel>> sourceModels)
+    {
+        static_assert(std::is_base_of<Model, TModel>::value, "Source model must be derived from Model");
+
+        auto models = QVector<Model&>{};
+        models.reserve(sourceModels.size());
+
+        for (const auto &sourceModel : sourceModels)
+        {
+            ASSERT(sourceModel != nullptr, "Source model cannot be nullptr")
+            if (sourceModel != nullptr)
+            {
+                models.push_back(*sourceModel);
+            }
+        }
+
+        return models;
+    }
+
+    DockSourceModelsUpdater();
+    ~DockSourceModelsUpdater() override = default;
+    CTOR_NO(DockSourceModelsUpdater)
+
+public:
+    MockUpOptionsModel& optionsModel() const;
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNode.h b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNode.h
new file mode 100644
index 0000000000000000000000000000000000000000..74a10de164e7a9a0cd4b54f02390e8020ae54b9f
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNode.h
@@ -0,0 +1,60 @@
+#pragma once
+
+#include "Helper.h"
+
+#include "Model.h"
+
+class MetaModel;
+class MockUpOptionsModel;
+
+class GroupNode final : public Model
+{
+Q_OBJECT
+
+public:
+    GroupNode(
+        GroupNode *parent,
+        const std::string &key,
+        const QString &value,
+        const std::vector<std::shared_ptr<MetaModel>> &metaModels
+    );
+
+    static GroupNode* createRoot(MockUpOptionsModel &parent, const std::vector<std::shared_ptr<MetaModel>> &metaModels);
+
+    ~GroupNode() override = default;
+
+    CTOR_NO(GroupNode)
+
+    const GroupNode *const parent;
+    const std::string key;
+    const QString value;
+    const size_t row;
+    const std::vector<std::shared_ptr<MetaModel>> metaModels;
+
+    [[nodiscard]] bool isRoot() const;
+    [[nodiscard]] bool isTerminal() const;
+
+    [[nodiscard]] bool visibility() const;
+    void setVisibility(bool value);
+
+    [[nodiscard]] bool selected() const;
+    void setSelected(bool value);
+
+    [[nodiscard]] bool expanded() const;
+    void setExpanded(bool value);
+
+    [[nodiscard]] QVector<GroupNode*> children() const;
+
+private:
+    QVector<GroupNode*> m_children;
+    std::atomic_bool m_visibility;
+    std::atomic_bool m_selected;
+    std::atomic_bool m_expanded;
+
+    GroupNode(MockUpOptionsModel &parent, const std::vector<std::shared_ptr<MetaModel>> &metaModels);
+
+signals:
+    void visibilityChanged(bool value) const;
+    void selectedChanged(bool value) const;
+    void expandedChanged(bool value) const;
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNodeView.h b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNodeView.h
new file mode 100644
index 0000000000000000000000000000000000000000..31ba4ab88eef9ba6d9146dd85422fe70d8aa8301
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/GroupNodeView.h
@@ -0,0 +1,23 @@
+#pragma once
+
+#include "Widget.h"
+
+class GroupNode;
+class QLayout;
+class QVBoxLayout;
+
+class GroupNodeView final : public Widget
+{
+Q_OBJECT
+public:
+    explicit GroupNodeView(const MockUpOptionsModel &optionsModel, GroupNode *node);
+
+private:
+    const MockUpOptionsModel &m_optionsModel;
+    GroupNode *const m_node;
+    QVBoxLayout *const m_contentLayout;
+
+    QLayout* populate() const;
+    QWidget* createHeader() const;
+    QWidget* createLeafView(MetaModel &metaModel) const;
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpContentView.h b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpContentView.h
new file mode 100644
index 0000000000000000000000000000000000000000..ba87f6758968450413258f7e6db812fab364610a
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpContentView.h
@@ -0,0 +1,20 @@
+#pragma once
+
+#include "Widget.h"
+
+class MockUpOptionsModel;
+class GroupNode;
+class QVBoxLayout;
+
+class MockUpContentView final : public Widget
+{
+Q_OBJECT
+
+    MockUpOptionsModel &m_optionsModel;
+    QVBoxLayout *const m_scrollAreaLayout;
+
+    void createContent();
+
+public:
+    explicit MockUpContentView(MockUpOptionsModel &optionsModel);
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockHeaderView.h b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockHeaderView.h
new file mode 100644
index 0000000000000000000000000000000000000000..633d7ebaae95ed684356813a4ed88bcb5487c88e
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockHeaderView.h
@@ -0,0 +1,28 @@
+#pragma once
+
+#include "Widget.h"
+
+class MockUpOptionsModel;
+class QToolButton;
+class SearchBox;
+
+class MockUpDockHeaderView final : public Widget
+{
+Q_OBJECT
+
+public:
+    explicit MockUpDockHeaderView(MockUpOptionsModel &optionsModel);
+
+private:
+    MockUpOptionsModel &m_optionsModel;
+    QToolButton *const m_sortButton;
+    QToolButton *const m_groupButton;
+    SearchBox *const m_searchBox;
+
+    void populateGroupMenu() const;
+    void populateSortMenu() const;
+
+private slots:
+    void onAvailableOptionsChanged() const;
+    void onSearchBoxChanged(const QString &text) const;
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockView.h b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockView.h
new file mode 100644
index 0000000000000000000000000000000000000000..0e88d4b46066c7442d60164f25b015d73f982264
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpDockView.h
@@ -0,0 +1,27 @@
+#pragma once
+
+#include "Helper.h"
+
+#include "DockWidgetBase.h"
+
+class DockSourceModelsUpdater;
+class MockUpContentView;
+class MockUpDockHeaderView;
+class MockUpOptionsModel;
+class PropertyModel;
+class DockSourceModelsUpdater;
+
+class MockUpDockView final : public DockWidgetBase
+{
+Q_OBJECT
+
+    std::unique_ptr<DockSourceModelsUpdater> m_sourceUpdater;
+    MockUpContentView *const m_content;
+    MockUpDockHeaderView *const m_header;
+
+public:
+    explicit MockUpDockView(std::unique_ptr<DockSourceModelsUpdater> sourceUpdater, QWidget *parent = nullptr);
+    ~MockUpDockView() override = default;
+
+    CTOR_NO(MockUpDockView)
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpOptionsModel.h b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpOptionsModel.h
new file mode 100644
index 0000000000000000000000000000000000000000..2b3d39e834feec456931f7bc2f7637064490310f
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/MockUpOptionsModel.h
@@ -0,0 +1,53 @@
+#pragma once
+
+#include "Helper.h"
+
+#include "Model.h"
+
+class MetaModel;
+
+struct SelectedOption
+{
+    const std::string key;
+    const size_t index;
+};
+
+// TODO: KM: Consider splitting into GroupTreeModel and HeaderModel
+class MockUpOptionsModel final : public Model
+{
+Q_OBJECT
+
+public:
+    CTOR_DEFAULT(MockUpOptionsModel)
+
+    [[nodiscard]] QVector<Model&> sourceModels() const;
+    void setSourceModels(QVector<Model&> models);
+
+    [[nodiscard]] std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> availableOptions() const;
+    void setAvailableOptions(
+        std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> availableOptions
+    );
+
+    [[nodiscard]] std::map<size_t, std::string> selectedGroupOptions() const;
+    void setSelectedGroupOptions(std::map<size_t, std::string> selectedOptions);
+
+    [[nodiscard]] std::optional<std::string> selectedSortOption() const;
+    void setSelectedSortOption(std::optional<std::string> selectedOptions);
+
+    void setRoot(GroupNode *node);
+    [[nodiscard]] GroupNode* root() const;
+
+private:
+    QVector<Model&> m_sourceModels;
+    std::map<const std::string, std::vector<std::shared_ptr<PropertyModel>>> m_availableOptions;
+    std::map<size_t, std::string> m_selectedGroupOptions;
+    std::optional<std::string> m_selectedSortOption;
+
+    GroupNode *m_root;
+
+signals:
+    void availableChanged() const;
+    void selectedChanged() const;
+    void sourceModelsChanged() const;
+    void rootChanged() const;
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.h b/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.h
new file mode 100644
index 0000000000000000000000000000000000000000..a610acd886a82ad56f05f578f8f9b1b08abf13bd
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.h
@@ -0,0 +1,46 @@
+#pragma once
+
+#include "Helper.h"
+
+class MockUpOptionsModel;
+class Model;
+class PropertyModel;
+class OrderModel;
+
+class OptionsService final
+{
+CTOR_STATIC(OptionsService)
+
+public:
+    using Properties = QVector<std::shared_ptr<PropertyModel>>;
+
+    static void populate(MockUpOptionsModel &optionsModel, const QVector<Model&> &sourceModels);
+    static void add(MockUpOptionsModel &optionsModel, Model &model);
+    static void remove(MockUpOptionsModel &optionsModel, Model &model);
+    static void clear(MockUpOptionsModel &optionsModel);
+
+    static void appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key);
+    static void appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key, size_t index);
+    static void removeGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string targetKey);
+
+    static void clearSelectedOptions(MockUpOptionsModel &optionsModel);
+
+    static GroupNode* createGroupTree(MockUpOptionsModel &optionsModel);
+
+    static void search(MockUpOptionsModel &optionsModel, const QString &text);
+
+private:
+    static std::map<QString, QVector<Model&>> generateValueModelsMap(
+        const QVector<Model&> &sourceModels,
+        const std::string &key
+    );
+
+    static void createNodes(
+        const MockUpOptionsModel &optionsModel,
+        GroupNode *parent,
+        QVector<std::string> keys,
+        const QVector<Model&> &sourceModels
+    );
+
+    static void sort(const MockUpOptionsModel &optionsModel, QVector<Model&> &unsortedModels);
+};
diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.h b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.h
new file mode 100644
index 0000000000000000000000000000000000000000..75eccfcb70692bf71b598af2f688d4dde4a5b663
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.h
@@ -0,0 +1,13 @@
+#pragma once
+
+#include "DockSourceModelsUpdater.h"
+
+class OrdersDockSourceUpdater final : public DockSourceModelsUpdater
+{
+Q_OBJECT
+
+public:
+    OrdersDockSourceUpdater();
+    ~OrdersDockSourceUpdater() override = default;
+    CTOR_NO(OrdersDockSourceUpdater)
+};
diff --git a/sources/libraries/frontend/viewservices/DocksViewService.h b/sources/libraries/frontend/viewservices/DocksViewService.h
index 45794650be4bc5534b431a3dd90f783b45d52db7..2b0aca168b817e4bafea9caedbb102192f9161e4 100644
--- a/sources/libraries/frontend/viewservices/DocksViewService.h
+++ b/sources/libraries/frontend/viewservices/DocksViewService.h
@@ -24,17 +24,18 @@ class VIEWSERVICES_EXPORT DocksViewService final
 CTOR_STATIC(DocksViewService)
 
 public:
-    static ads::CDockAreaWidget* createPermanentDock(QAction *action,
-                                                     PermanentDock dockType,
-                                                     const QString &dockName,
-                                                     const QIcon &icon,
-                                                     QWidget *childWidget,
-                                                     ads::CDockManager *dockManager,
-                                                     ads::CDockAreaWidget *addDockToAreaWidget,
-                                                     ads::CDockWidget::eInsertMode insertMode =
-                                                             ads::CDockWidget::AutoScrollArea,
-                                                     ads::DockWidgetArea addDockToArea =
-                                                             ads::DockWidgetArea::NoDockWidgetArea, bool isVisible= true);
+    static ads::CDockAreaWidget* createPermanentDock(
+        QAction *action,
+        PermanentDock dockType,
+        const QString &dockName,
+        const QIcon &icon,
+        QWidget *childWidget,
+        ads::CDockManager *dockManager,
+        ads::CDockAreaWidget *addDockToAreaWidget,
+        ads::CDockWidget::eInsertMode insertMode = ads::CDockWidget::AutoScrollArea,
+        ads::DockWidgetArea addDockToArea = ads::DockWidgetArea::NoDockWidgetArea,
+        bool isVisible = true
+    );
 
     static std::shared_ptr<RuntimeDockModel> createRuntimeDock(RuntimeDock dockType,
                                                                const QString &dockName,
diff --git a/sources/libraries/frontend/widgets/expander/ExpanderButton.h b/sources/libraries/frontend/widgets/expander/ExpanderButton.h
index 02fd10f3051fa0322643d56eca514b52efa6dc90..dd0915d711cac4dc421b8bed2127646eba2b71fb 100644
--- a/sources/libraries/frontend/widgets/expander/ExpanderButton.h
+++ b/sources/libraries/frontend/widgets/expander/ExpanderButton.h
@@ -1,9 +1,9 @@
 #pragma once
 
-#include <QPushButton>
-
 #include "Helper.h"
 
+#include <QPushButton>
+
 #include "widgets_export.h"
 
 class WIDGETS_EXPORT ExpanderButton final : public QPushButton
@@ -11,7 +11,7 @@ class WIDGETS_EXPORT ExpanderButton final : public QPushButton
 Q_OBJECT
 
 public:
-    ExpanderButton(const QString &text, QWidget *parent = nullptr);
+    explicit ExpanderButton(const QString &text, QWidget *parent = nullptr);
     ~ExpanderButton() = default;
     CTOR_NO(ExpanderButton)
 };

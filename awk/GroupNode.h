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
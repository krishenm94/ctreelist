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

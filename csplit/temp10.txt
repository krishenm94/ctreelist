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

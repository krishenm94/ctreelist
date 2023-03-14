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

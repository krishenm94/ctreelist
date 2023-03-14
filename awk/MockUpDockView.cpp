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

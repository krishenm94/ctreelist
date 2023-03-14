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

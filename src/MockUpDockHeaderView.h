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

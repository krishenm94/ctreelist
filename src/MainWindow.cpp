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

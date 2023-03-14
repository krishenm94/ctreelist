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

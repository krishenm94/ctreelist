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

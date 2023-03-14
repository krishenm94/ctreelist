diff --git a/sources/libraries/applications/dtectx/main/MainWindow.h b/sources/libraries/applications/dtectx/main/MainWindow.h
index fe0c355a0273e27782dd5bba1e845d8e1df56451..78fe26bbdf65f3608d34e834fcb225fa4cc61f69 100644
--- a/sources/libraries/applications/dtectx/main/MainWindow.h
+++ b/sources/libraries/applications/dtectx/main/MainWindow.h
@@ -52,6 +52,7 @@ private:
 
     QAction *m_workspaceDockAction{nullptr};
     QAction *m_ordersDockAction{nullptr};
+    QAction *m_mockUpDockAction{nullptr};
     QAction *m_devicesDockAction{nullptr};
     QAction *m_notificationsDockAction{nullptr};
     QAction *m_ordersPropertiesDockAction{nullptr};

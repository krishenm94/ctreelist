diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.h b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.h
new file mode 100644
index 0000000000000000000000000000000000000000..75eccfcb70692bf71b598af2f688d4dde4a5b663
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.h
@@ -0,0 +1,13 @@
+#pragma once
+
+#include "DockSourceModelsUpdater.h"
+
+class OrdersDockSourceUpdater final : public DockSourceModelsUpdater
+{
+Q_OBJECT
+
+public:
+    OrdersDockSourceUpdater();
+    ~OrdersDockSourceUpdater() override = default;
+    CTOR_NO(OrdersDockSourceUpdater)
+};

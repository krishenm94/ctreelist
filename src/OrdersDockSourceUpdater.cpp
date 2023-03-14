diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.cpp b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..ba351c7519a158480cf6ba2e885f03b33257b7aa
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/OrdersDockSourceUpdater.cpp
@@ -0,0 +1,40 @@
+#include "OrdersDockSourceUpdater.h"
+
+#include "ModelStore.h"
+#include "OrdersModel.h"
+
+OrdersDockSourceUpdater::OrdersDockSourceUpdater()
+{
+    const auto &orders = ModelStore::get<OrdersModel>();
+
+    populate(upcast(orders.orders()));
+
+    connect(orders,
+            &OrdersModel::added,
+            [this](const auto &order)
+            {
+                ASSERT(order != nullptr, "Order cannot be nullptr")
+                if (order != nullptr)
+                {
+                    add(*order);
+                }
+            });
+
+    connect(orders,
+            &OrdersModel::removed,
+            [this](const auto &order)
+            {
+                ASSERT(order != nullptr, "Order cannot be nullptr")
+                if (order != nullptr)
+                {
+                    remove(*order);
+                }
+            });
+
+    connect(orders,
+            &OrdersModel::cleared,
+            [this]()
+            {
+                clear();
+            });
+}

diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.h b/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.h
new file mode 100644
index 0000000000000000000000000000000000000000..f77a634db344c04846e97c6a60215b86b6ea1ff6
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/DockSourceModelsUpdater.h
@@ -0,0 +1,57 @@
+#pragma once
+
+#include "Connector.h"
+
+#include <QObject>
+#include <QVector>
+
+class MockUpOptionsModel;
+class Model;
+
+class Dispatcher : public Connector<QObject>
+{
+Q_OBJECT
+
+protected:
+    CTOR_DEFAULT(Dispatcher)
+};
+
+class DockSourceModelsUpdater : public Dispatcher
+{
+Q_OBJECT
+
+    std::unique_ptr<MockUpOptionsModel> m_optionsModel;
+
+protected:
+    void populate(const QVector<Model&> &models) const;
+    void add(Model &model) const;
+    void remove(Model &model) const;
+    void clear() const;
+
+    template<typename TModel>
+    static QVector<Model&> upcast(std::vector<std::shared_ptr<TModel>> sourceModels)
+    {
+        static_assert(std::is_base_of<Model, TModel>::value, "Source model must be derived from Model");
+
+        auto models = QVector<Model&>{};
+        models.reserve(sourceModels.size());
+
+        for (const auto &sourceModel : sourceModels)
+        {
+            ASSERT(sourceModel != nullptr, "Source model cannot be nullptr")
+            if (sourceModel != nullptr)
+            {
+                models.push_back(*sourceModel);
+            }
+        }
+
+        return models;
+    }
+
+    DockSourceModelsUpdater();
+    ~DockSourceModelsUpdater() override = default;
+    CTOR_NO(DockSourceModelsUpdater)
+
+public:
+    MockUpOptionsModel& optionsModel() const;
+};

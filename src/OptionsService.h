diff --git a/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.h b/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.h
new file mode 100644
index 0000000000000000000000000000000000000000..a610acd886a82ad56f05f578f8f9b1b08abf13bd
--- /dev/null
+++ b/sources/libraries/frontend/docks/permanent/orders/mockup/OptionsService.h
@@ -0,0 +1,46 @@
+#pragma once
+
+#include "Helper.h"
+
+class MockUpOptionsModel;
+class Model;
+class PropertyModel;
+class OrderModel;
+
+class OptionsService final
+{
+CTOR_STATIC(OptionsService)
+
+public:
+    using Properties = QVector<std::shared_ptr<PropertyModel>>;
+
+    static void populate(MockUpOptionsModel &optionsModel, const QVector<Model&> &sourceModels);
+    static void add(MockUpOptionsModel &optionsModel, Model &model);
+    static void remove(MockUpOptionsModel &optionsModel, Model &model);
+    static void clear(MockUpOptionsModel &optionsModel);
+
+    static void appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key);
+    static void appendGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string key, size_t index);
+    static void removeGroupSelectedOption(MockUpOptionsModel &optionsModel, std::string targetKey);
+
+    static void clearSelectedOptions(MockUpOptionsModel &optionsModel);
+
+    static GroupNode* createGroupTree(MockUpOptionsModel &optionsModel);
+
+    static void search(MockUpOptionsModel &optionsModel, const QString &text);
+
+private:
+    static std::map<QString, QVector<Model&>> generateValueModelsMap(
+        const QVector<Model&> &sourceModels,
+        const std::string &key
+    );
+
+    static void createNodes(
+        const MockUpOptionsModel &optionsModel,
+        GroupNode *parent,
+        QVector<std::string> keys,
+        const QVector<Model&> &sourceModels
+    );
+
+    static void sort(const MockUpOptionsModel &optionsModel, QVector<Model&> &unsortedModels);
+};

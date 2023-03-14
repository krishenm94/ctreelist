diff --git a/sources/libraries/frontend/widgets/expander/ExpanderButton.h b/sources/libraries/frontend/widgets/expander/ExpanderButton.h
index 02fd10f3051fa0322643d56eca514b52efa6dc90..dd0915d711cac4dc421b8bed2127646eba2b71fb 100644
--- a/sources/libraries/frontend/widgets/expander/ExpanderButton.h
+++ b/sources/libraries/frontend/widgets/expander/ExpanderButton.h
@@ -1,9 +1,9 @@
 #pragma once
 
-#include <QPushButton>
-
 #include "Helper.h"
 
+#include <QPushButton>
+
 #include "widgets_export.h"
 
 class WIDGETS_EXPORT ExpanderButton final : public QPushButton
@@ -11,7 +11,7 @@ class WIDGETS_EXPORT ExpanderButton final : public QPushButton
 Q_OBJECT
 
 public:
-    ExpanderButton(const QString &text, QWidget *parent = nullptr);
+    explicit ExpanderButton(const QString &text, QWidget *parent = nullptr);
     ~ExpanderButton() = default;
     CTOR_NO(ExpanderButton)
 };

#pragma once

#include "DockSourceModelsUpdater.h"

class OrdersDockSourceUpdater final : public DockSourceModelsUpdater
{
Q_OBJECT

public:
    OrdersDockSourceUpdater();
    ~OrdersDockSourceUpdater() override = default;
    CTOR_NO(OrdersDockSourceUpdater)
};

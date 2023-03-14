#pragma once

#include "Helper.h"

#include "DockWidgetBase.h"

class DockSourceModelsUpdater;
class MockUpContentView;
class MockUpDockHeaderView;
class MockUpOptionsModel;
class PropertyModel;
class DockSourceModelsUpdater;

class MockUpDockView final : public DockWidgetBase
{
Q_OBJECT

    std::unique_ptr<DockSourceModelsUpdater> m_sourceUpdater;
    MockUpContentView *const m_content;
    MockUpDockHeaderView *const m_header;

public:
    explicit MockUpDockView(std::unique_ptr<DockSourceModelsUpdater> sourceUpdater, QWidget *parent = nullptr);
    ~MockUpDockView() override = default;

    CTOR_NO(MockUpDockView)
};

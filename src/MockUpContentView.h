#pragma once

#include "Widget.h"

class MockUpOptionsModel;
class GroupNode;
class QVBoxLayout;

class MockUpContentView final : public Widget
{
Q_OBJECT

    MockUpOptionsModel &m_optionsModel;
    QVBoxLayout *const m_scrollAreaLayout;

    void createContent();

public:
    explicit MockUpContentView(MockUpOptionsModel &optionsModel);
};

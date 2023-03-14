#pragma once

#include "Widget.h"

class GroupNode;
class QLayout;
class QVBoxLayout;

class GroupNodeView final : public Widget
{
Q_OBJECT
public:
    explicit GroupNodeView(const MockUpOptionsModel &optionsModel, GroupNode *node);

private:
    const MockUpOptionsModel &m_optionsModel;
    GroupNode *const m_node;
    QVBoxLayout *const m_contentLayout;

    QLayout* populate() const;
    QWidget* createHeader() const;
    QWidget* createLeafView(MetaModel &metaModel) const;
};

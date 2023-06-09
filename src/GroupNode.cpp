#include "GroupNode.h"

#include "MockUpOptionsModel.h"

GroupNode::GroupNode(
    GroupNode *parent,
    const std::string &key,
    const QString &value,
    const std::vector<std::shared_ptr<MetaModel>> &metaModels
) :
    parent{parent},
    key{key},
    value{value},
    row{parent->row + 1},
    metaModels{metaModels}
{
    parent->m_children.push_back(this);
    setParent(parent);
}

GroupNode* GroupNode::createRoot(MockUpOptionsModel &parent, const std::vector<std::shared_ptr<MetaModel>> &metaModels)
{
    return new GroupNode(parent, metaModels);
}

GroupNode::GroupNode(MockUpOptionsModel &parent, const std::vector<std::shared_ptr<MetaModel>> &metaModels) :
    parent{nullptr},
    value(""),
    row{0},
    metaModels{metaModels}
{
    parent.setRoot(this);
    setParent(&parent);
}

bool GroupNode::isRoot() const
{
    ASSERT(!((parent == nullptr) ^ (row == 0)), "If node has no parent, then it's row must be zero and vice versa.")
    return row == 0 && parent == nullptr;
}

bool GroupNode::isTerminal() const
{
    return m_children.empty();
}

bool GroupNode::visibility() const
{
    return m_visibility;
}

void GroupNode::setVisibility(bool value)
{
    if (m_visibility == value)
    {
        return;
    }

    m_visibility = value;

    emit visibilityChanged(value);
    emit dataChanged();
}

bool GroupNode::selected() const
{
    return m_selected;
}

void GroupNode::setSelected(bool value)
{
    if (m_selected == value)
    {
        return;
    }

    m_selected = value;

    emit selectedChanged(value);
    emit dataChanged();
}

bool GroupNode::expanded() const
{
    return m_expanded;
}

void GroupNode::setExpanded(bool value)
{
    if (m_expanded == value)
    {
        return;
    }

    m_expanded = value;

    emit expandedChanged(value);
    emit dataChanged();
}

QVector<GroupNode*> GroupNode::children() const
{
    const auto lock = lockForRead();
    return m_children;
}

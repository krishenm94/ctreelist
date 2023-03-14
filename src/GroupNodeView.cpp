#include "GroupNodeView.h"

#include "OrderModel.h"

#include "ExpanderButton.h"
#include "GroupNode.h"

#include <QLabel>
#include <QVBoxLayout>

GroupNodeView::GroupNodeView(const MockUpOptionsModel &optionsModel, GroupNode *node) :
    m_optionsModel{optionsModel},
    m_node(node),
    m_contentLayout{new QVBoxLayout()}
{
    setLayout(populate());
}

QLayout* GroupNodeView::populate() const
{
    auto *layout = new QVBoxLayout();
    layout->setContentsMargins(QMargins{});

    if (!m_node->isRoot())
    {
        layout->addWidget(createHeader());
    }

    m_contentLayout->setContentsMargins(QMargins{});

    if (m_node->isTerminal())
    {
        for (const auto &metaModel : m_node->metaModels)
        {
            m_contentLayout->addWidget(createLeafView(*metaModel));
        }
    }
    else
    {
        for (auto &child : m_node->children())
        {
            m_contentLayout->addWidget(new GroupNodeView(m_optionsModel, child));
        }
    }

    auto *const wrapperLayout = new QHBoxLayout();
    wrapperLayout->setContentsMargins(QMargins());

    if (!m_node->isRoot())
    {
        wrapperLayout->addWidget(new QLabel("-"));
    }

    wrapperLayout->addLayout(m_contentLayout);
    layout->addLayout(wrapperLayout);
    return layout;
}

QWidget* GroupNodeView::createHeader() const
{
    // TODO: new_list: create factory
    auto *const expander = new ExpanderButton(m_node->value);

    connect(expander,
            &QPushButton::released,
            [this]()
            {
                m_node->setExpanded(m_node->expanded());
            });

    return expander;
}

QWidget* GroupNodeView::createLeafView(MetaModel &metaModel) const
{
    // TODO: new_list: create factory
    return new QLabel(dynamic_cast<OrderModel&>(metaModel).name());
}

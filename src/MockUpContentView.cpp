#include "MockUpContentView.h"

#include "MockUpOptionsModel.h"

#include "OptionsService.h"

#include "Common.h"
#include "GroupNodeView.h"
#include "UiUtilities.h"

#include <QScrollArea>
#include <QVBoxLayout>

MockUpContentView::MockUpContentView(MockUpOptionsModel &optionsModel):
    m_optionsModel{optionsModel},
    m_scrollAreaLayout{new QVBoxLayout()}
{
    auto *const scrollArea = Common::uiUtilities().wrapInScrollArea(m_scrollAreaLayout);

    auto *const layout = new QVBoxLayout();
    layout->setContentsMargins(QMargins());
    layout->addWidget(scrollArea);

    setLayout(layout);

    connect(m_optionsModel,
            &MockUpOptionsModel::selectedChanged,
            [this]()
            {
                createContent();
            });

    createContent();
}

void MockUpContentView::createContent()
{
    auto *const root = OptionsService::createGroupTree(m_optionsModel);
    auto *const rootView = new GroupNodeView(m_optionsModel, root);

    m_scrollAreaLayout->setContentsMargins(QMargins());
    clearLayout(m_scrollAreaLayout);
    m_scrollAreaLayout->addWidget(rootView);
    m_scrollAreaLayout->addStretch(1);
}

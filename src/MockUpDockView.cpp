#include "MockUpDockView.h"

#include "DockSourceModelsUpdater.h"
#include "MockUpContentView.h"
#include "MockUpDockHeaderView.h"

#include <QVBoxLayout>

MockUpDockView::MockUpDockView(std::unique_ptr<DockSourceModelsUpdater> sourceUpdater, QWidget *parent):
    DockWidgetBase{parent},
    m_sourceUpdater{std::move(sourceUpdater)},
    m_content{new MockUpContentView(sourceUpdater->optionsModel())},
    m_header{new MockUpDockHeaderView(sourceUpdater->optionsModel())}
{
    auto *const layout = new QVBoxLayout();
    layout->setContentsMargins(QMargins());
    layout->addWidget(m_header);
    layout->addWidget(m_content);
    setLayout(layout);
}

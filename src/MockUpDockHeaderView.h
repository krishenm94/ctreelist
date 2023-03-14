#pragma once

#include "Widget.h"

class MockUpOptionsModel;
class QToolButton;
class SearchBox;

class MockUpDockHeaderView final : public Widget
{
Q_OBJECT

public:
    explicit MockUpDockHeaderView(MockUpOptionsModel &optionsModel);

private:
    MockUpOptionsModel &m_optionsModel;
    QToolButton *const m_sortButton;
    QToolButton *const m_groupButton;
    SearchBox *const m_searchBox;

    void populateGroupMenu() const;
    void populateSortMenu() const;

private slots:
    void onAvailableOptionsChanged() const;
    void onSearchBoxChanged(const QString &text) const;
};

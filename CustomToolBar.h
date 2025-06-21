#ifndef CUSTOMTOOLBAR_H
#define CUSTOMTOOLBAR_H

#include "ChessBoardWidget.h"

#include <QToolBar>
#include <QAction>

class CustomToolBar : public QToolBar
{
    Q_OBJECT

public:
    void addSpacer(QToolBar* toolbar, int width);

    explicit CustomToolBar(QWidget *parent = nullptr, ChessBoardWidget *bWidget = nullptr);

private:
    ChessBoardWidget *boardWidget = nullptr;
};

#endif // CUSTOMTOOLBAR_H

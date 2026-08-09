
#include "CustomToolBar.h"
#include "menu_file.h"
#include "menu_move.h"
#include "menu_position.h"
#include "menu_engine.h"


void CustomToolBar::addSpacer(QToolBar* toolbar, int width)
{
    QWidget* spacer = new QWidget(toolbar);
    spacer->setFixedWidth(width);
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);
}

CustomToolBar::CustomToolBar(QWidget *parent, ChessBoardWidget *bWidget)
    : QToolBar(parent), boardWidget(bWidget)
{
    setMovable(true);
    setFloatable(true);
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addSpacer(this, 10);
    addFileActionsToToolBar(this, parent);
    addSpacer(this, 10);

    addSeparator();
    addSeparator();

    addSpacer(this, 10);
    addMoveActionsToToolBar(this, parent);
    addSpacer(this, 10);

    addSeparator();
    addSeparator();

    addSpacer(this, 10);
    addPositionActionsToToolBar(this, parent);
    addSpacer(this, 10);

    addSeparator();
    addSeparator();

    addSpacer(this, 10);

    MenuEngine *engineMenu = new MenuEngine(bWidget);
    // qDebug() << "[CustomToolBar] bWidget ptr =" << bWidget;

    engineMenu->addEngineActionsToToolBar(this, parent);
    addSpacer(this, 10);

    addSeparator();
    addSeparator();
}


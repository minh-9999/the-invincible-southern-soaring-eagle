#include "CustomToolBar_2.h"
#include "menu_connection.h"

#include <QComboBox>


CustomToolBar_2::CustomToolBar_2(CustomToolBar *custom, QWidget *parent)
    : QToolBar(parent), c1(custom)
{
    setMovable(true);
    setFloatable(true);
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    if (c1)
        c1->addSpacer(this, 10);

    // Create combo box
    QComboBox *siteSelector = new QComboBox(this);
    siteSelector->addItem("天天象棋 - xiangqi.qq.com");
    siteSelector->addItem("ZIGA");
    siteSelector->addItem("PlayOK");
    siteSelector->addItem("ClubXiangqi");
    siteSelector->addItem("Xiangqi.com");
    siteSelector->addItem("MoveskyXiangqi");

    siteSelector->setStyleSheet("QComboBox { color: blue; font-weight: bold; }");

    addWidget(siteSelector);

    addSeparator();
    if (c1)
        c1->addSpacer(this, 4);

    MenuConnection *connMenu = new MenuConnection(parent);
    connMenu->addConnectActionsToToolBar(this, parent);

    if (c1)
        c1->addSpacer(this, 10);

    addSeparator();
    addSeparator();
}

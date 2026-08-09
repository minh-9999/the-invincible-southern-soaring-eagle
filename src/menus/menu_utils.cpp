#include "menu_utils.h"

#include <QWidgetAction>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

void MenuUtils::addFullWidthSeparator(QMenu* menu, int height)
{
    if (!menu) return;

    QLabel* separatorLabel = new QLabel(menu);
    separatorLabel->setFixedHeight(height > 0 ? height : 2);
    separatorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    separatorLabel->setStyleSheet("background-color: #a0a0a0; margin: 0; padding: 0;");

    QWidgetAction* separatorAction = new QWidgetAction(menu);
    separatorAction->setDefaultWidget(separatorLabel);
    menu->addAction(separatorAction);
}





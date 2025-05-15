#include "menu_utils.h"

#include <QWidgetAction>
#include <QFrame>
#include <QVBoxLayout>

void MenuUtils::addFullWidthSeparator(QMenu* menu, int height)
{
    if (!menu) return;

    QWidget* container = new QWidget(menu);
    container->setFixedHeight(height);

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(8, 0, 8, 0);
    layout->setSpacing(0);

    QFrame* line = new QFrame(container);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #c0c0c0;");
    line->setFixedHeight(height);

    layout->addWidget(line);
    container->setLayout(layout);

    QWidgetAction* separator = new QWidgetAction(menu);
    separator->setDefaultWidget(container);

    menu->addAction(separator);
}





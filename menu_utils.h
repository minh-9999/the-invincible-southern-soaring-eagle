#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include <QMenu>
#include <QProxyStyle>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

class MenuUtils
{
public:
    static void addFullWidthSeparator(QMenu* menu, int height = 2);
};

#endif // MENU_UTILS_H


#ifndef CUSTOMTOOLBAR_2_H
#define CUSTOMTOOLBAR_2_H

#include <QToolBar>
#include <QAction>

#include "CustomToolBar.h"

class CustomToolBar_2 : public QToolBar
{
    Q_OBJECT

public:

    explicit CustomToolBar_2(CustomToolBar *custom, QWidget *parent = nullptr);

    CustomToolBar *c1 = nullptr;

};


#endif // CUSTOMTOOLBAR_2_H

#ifndef MENU_BOOK_H
#define MENU_BOOK_H

#include <QObject>
#include <QMainWindow>

class MenuBook : public QObject
{
    Q_OBJECT  

public:
    explicit MenuBook(QObject *parent = nullptr);
    void bookMenus(QMainWindow* window);

private:
    bool openBookEnabled;

private slots:
    void onOpenBookSettings();
    void onAddMovesToBook();
    void onEnableOpenBookToggled(bool enabled);
};

#endif // MENU_BOOK_H

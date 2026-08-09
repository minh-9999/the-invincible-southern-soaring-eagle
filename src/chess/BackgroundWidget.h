#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QStringList>
#include <QPixmap>

class BackgroundWidget : public QWidget
{
    Q_OBJECT

private:

    QString baseImagePath;

public:
    QAction *toggleBackgroundAction;
    QAction *changeBackgroundAction;

    explicit BackgroundWidget(QWidget *parent = nullptr);

    bool isBackgroundVisible = true;  // Variable to store background display state
    QPixmap backgroundImage;
    QStringList backgroundImages;
    int currentBackgroundIndex = 0;

protected:
    void paintEvent(QPaintEvent *event) override;
    void showContextMenu(const QPoint &pos);
    void changeBackground();
    void initBackgroundImages(const QString &folderPath);
    void toggleBackground();

signals:
    void backgroundToggled(bool visible);
};

#endif // BACKGROUNDWIDGET_H
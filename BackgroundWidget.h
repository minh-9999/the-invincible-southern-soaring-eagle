#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>


class BackgroundWidget : public QWidget
{
    Q_OBJECT

private:
    bool isBackgroundVisible = true;  // Variable to store background display state
    QPixmap backgroundImage;
    QStringList backgroundImages;
    int currentBackgroundIndex = 0;

    QString baseImagePath;

public:
    QAction *toggleBackgroundAction;
    QAction *changeBackgroundAction;

    explicit BackgroundWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    void showContextMenu(const QPoint &pos);

    void changeBackground();

    void initBackgroundImages(const QString &folderPath);

    void toggleBackground();

};

#endif // BACKGROUNDWIDGET_H

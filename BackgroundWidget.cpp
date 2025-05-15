
#include "BackgroundWidget.h"
#include <QPainter>
#include <QMenu>
#include <QFileDialog>
#include <QPushButton>

BackgroundWidget::BackgroundWidget(QWidget *parent)
    : QWidget(parent)
{
    backgroundImage = QPixmap(":/img/bg/bg5.jpg");  // default
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    changeBackgroundAction = new QAction(QIcon(":/icons/change_background_icon.png"),
                                         tr("Change Background..."), this);
    connect(changeBackgroundAction, &QAction::triggered, this, &BackgroundWidget::changeBackground);

    toggleBackgroundAction = new QAction(tr("Toggle Background"), this);
    toggleBackgroundAction->setCheckable(true);
    connect(toggleBackgroundAction, &QAction::triggered, this, &BackgroundWidget::toggleBackground);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &BackgroundWidget::showContextMenu);

    initBackgroundImages(":/img/bg");
    update();
}


void BackgroundWidget::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Background Menu"), this);

    contextMenu.addAction(changeBackgroundAction);

    toggleBackgroundAction->setChecked(isBackgroundVisible);
    contextMenu.addAction(toggleBackgroundAction);

    // display menu
    contextMenu.exec(mapToGlobal(pos));
}

void BackgroundWidget::changeBackground()
{
    // QString fileName = QFileDialog::getOpenFileName(
    //     this, tr("Open Image"), "",
    //     tr("Images (*.png *.jpg *.jpeg *.bmp *.gif *.webp *.tiff *.svg *.heic *.heif);;All Files (*)")
    //     );

    // if (!fileName.isEmpty())
    // {
    //     backgroundImage.load(fileName);
    //     update(); // Trigger repaint
    // }

    if (backgroundImages.isEmpty())
        return;

    QString imagePath = QDir("path/to/background/folder").filePath(backgroundImages[currentBackgroundIndex]);
    backgroundImage.load(imagePath);
    update(); // Trigger repaint

    currentBackgroundIndex = (currentBackgroundIndex + 1) % backgroundImages.size();
}

void BackgroundWidget ::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // draw background
    if (!backgroundImage.isNull())
    {
        // QPixmap scaled = backgroundImage.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled = backgroundImage.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        // Upper-right corner coordinates
        int x = width() - scaled.width();
        int y = 0;

        if (isBackgroundVisible && !backgroundImage.isNull())
            painter.drawPixmap(x, y, scaled);
    }
}

void BackgroundWidget::initBackgroundImages(const QString &folderPath)
{
    QDir dir(folderPath);
    QStringList filters = {"*.png", "*.jpg", "*.jpeg", "*.bmp", "*.gif", "*.webp", "*.tiff", "*.svg", "*.heic", "*.heif"};
    backgroundImages = dir.entryList(filters, QDir::Files);

    for (int i = 0; i < backgroundImages.size(); ++i)
        backgroundImages[i] = dir.filePath(backgroundImages[i]); // convert to full path

    currentBackgroundIndex = 0;
}

void BackgroundWidget::toggleBackground()
{
    isBackgroundVisible = !isBackgroundVisible;  // Change state
    update();  // Request widget redraw
}



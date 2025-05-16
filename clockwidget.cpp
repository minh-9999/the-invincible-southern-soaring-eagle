#include "clockwidget.h"

#include <QVBoxLayout>
#include <QFont>
#include <QDateTime>


ClockWidget::ClockWidget(QWidget *parent) : QWidget(parent), seconds1(0), seconds2(0)
{
    labelPlayer1 = new QLabel("00:00:00");
    labelPlayer2 = new QLabel("00:00:00");

    labelPlayer1->setStyleSheet("color: blue; font-size: 20px; font-weight: bold; margin: 0px; padding: 0px;");
    labelPlayer2->setStyleSheet("color: red; font-size: 20px; font-weight: bold; margin: 0px; padding: 0px;");
    labelPlayer1->setAlignment(Qt::AlignCenter);
    labelPlayer2->setAlignment(Qt::AlignCenter);

    labelPlayer1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelPlayer2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelPlayer1->setFixedSize(100, 40);
    // labelPlayer1->setMaximumHeight(40);
    labelPlayer2->setFixedSize(100, 40);
    // labelPlayer2->setMaximumHeight(40);


    QString titleText1 = QStringLiteral("將 %1").arg(tr("Player1"));
    QString titleText2 = QStringLiteral("帥 %1").arg(tr("Player2"));

    QLabel *title1 = new QLabel(titleText1);
    QLabel *title2 = new QLabel(titleText2);

    title1->setStyleSheet("color: blue; font-size: 16px;");
    title2->setStyleSheet("color: red; font-size: 16px;");

    title1->setAlignment(Qt::AlignCenter);
    title2->setAlignment(Qt::AlignCenter);
    title1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title1->setFixedSize(100, 40);
    title2->setFixedSize(100, 40);

    // title1->setMaximumHeight(40);
    // title2->setMaximumHeight(40);


    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(title1);
    vbox1->addWidget(labelPlayer1);
    vbox1->setSpacing(2);
    // vbox1->setContentsMargins(10, 5, 10, 5);
    vbox1->setContentsMargins(2, 2, 2, 2);

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(title2);
    vbox2->addWidget(labelPlayer2);
    vbox2->setSpacing(2);
    vbox2->setContentsMargins(10, 5, 10, 5);
    vbox1->setContentsMargins(2, 2, 2, 2);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(vbox1);
    mainLayout->addLayout(vbox2);
    mainLayout->setSpacing(2);
    // mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setContentsMargins(10, 6, 10, 6);

    setLayout(mainLayout);
    setStyleSheet("background-color: lightgray; border: 2px solid gray; border-radius: 8px;");

    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    connect(timer1, &QTimer::timeout, this, &ClockWidget::updatePlayer1);
    connect(timer2, &QTimer::timeout, this, &ClockWidget::updatePlayer2);
}

void ClockWidget::startTimers()
{
    timer1->start(1000);
    timer2->start(1000);
}

void ClockWidget::stopTimers()
{
    timer1->stop();
    timer2->stop();
}

void ClockWidget::resetTimers()
{
    seconds1 = 0;
    seconds2 = 0;
    labelPlayer1->setText("00:00:00");
    labelPlayer2->setText("00:00:00");
}

void ClockWidget::updatePlayer1()
{
    seconds1++;
    QTime time(0, 0);
    labelPlayer1->setText(time.addSecs(seconds1).toString("hh:mm:ss"));
}

void ClockWidget::updatePlayer2()
{
    seconds2++;
    QTime time(0, 0);
    labelPlayer2->setText(time.addSecs(seconds2).toString("hh:mm:ss"));
}

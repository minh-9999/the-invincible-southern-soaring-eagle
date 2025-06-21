#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>

class ClockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClockWidget(QWidget *parent = nullptr);

    void startTimers();
    void stopTimers();
    void resetTimers();

private slots:
    void updatePlayer1();
    void updatePlayer2();

private:
    QLabel *labelPlayer1;
    QLabel *labelPlayer2;
    QTimer *timer1;
    QTimer *timer2;
    int seconds1;
    int seconds2;
};

#endif // CLOCKWIDGET_H

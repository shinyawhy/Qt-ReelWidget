#ifndef REELWIDGET_H
#define REELWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QQueue>
#include <QTime>
#include <QRandomGenerator>
class ReelWidget : public QWidget
{
    Q_OBJECT

public:
    ReelWidget(QWidget *parent = nullptr);
    void setNewNumber(uint newNumber);

protected:
    void paintEvent(QPaintEvent *event)override;

private:
    uint lastNumber{0};
    uint currentNumber{1};
    const QPixmap bg;
    bool isRuning{false};
    QTimer timer;
    enum class state
    {
        noRun,
        runing_up,
        runing_down
    };
    state widgetState{state::noRun};
    QRect whenRuningImageRect_Current;
    QRect whenRuningImageRect_Last;
    void onTimer();
    void drawAUnit(const QRect &rect, QPainter * painter, uint number);
};

class testReelWidget : public QWidget
{
    Q_OBJECT

public:
    testReelWidget(QWidget *parent = nullptr);
    void setValue(int value);

protected:
    QSize sizeHint() const override;

private:
    QList<ReelWidget *> list;
    QTimer timer;
    void onTimer();
};


#endif // REELWIDGET_H

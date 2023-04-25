#include "reelwidget.h"

int rateFactor = 5;

using namespace  std;

ReelWidget::ReelWidget(QWidget *parent) : QWidget(parent), bg{":/icon/bg.png"}
{
    setFixedSize(bg.rect().size());
    auto font = this->font();
    font.setPixelSize(36);
    setFont(font);

    connect(&timer,&QTimer::timeout,this,&ReelWidget::onTimer);
    timer.setInterval(40);
}

void ReelWidget::setNewNumber(uint newNumber)
{
    if(this->widgetState == state::noRun && newNumber != currentNumber)
    {
        lastNumber = this->currentNumber;
        this->currentNumber = newNumber;
        widgetState = lastNumber > currentNumber ? state::runing_down : state::runing_up;
        whenRuningImageRect_Current = bg.rect();
        whenRuningImageRect_Last = whenRuningImageRect_Current;
        whenRuningImageRect_Current.moveTop(widgetState == state::runing_up ? bg.rect().height() : -bg.rect().height());
        timer.start();
    }
}

void ReelWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(this->font());
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    if(widgetState == state::noRun)
    {
        drawAUnit(event->rect(),&painter,currentNumber);
    }
    else
    {
        drawAUnit(whenRuningImageRect_Last,&painter,lastNumber);
        drawAUnit(whenRuningImageRect_Current,&painter,currentNumber);
    }
}

void ReelWidget::onTimer()
{
    if(widgetState == state::runing_up)
    {
        auto currentTop = whenRuningImageRect_Current.top() - rateFactor;
        if(currentTop <= 0)
        {
            timer.stop();
            widgetState = state::noRun;
        }
        else
        {
            whenRuningImageRect_Last.moveTop(whenRuningImageRect_Last.top() - rateFactor);
            whenRuningImageRect_Current.moveTop(currentTop);
        }
    }
    else
    {
        auto currentTop = whenRuningImageRect_Current.top() + rateFactor;
        if(currentTop >= 0)
        {
            timer.stop();
            widgetState = state::noRun;
        }
        else
        {
            whenRuningImageRect_Last.moveTop(whenRuningImageRect_Last.top() + rateFactor);
            whenRuningImageRect_Current.moveTop(currentTop);
        }
    }
    update();
}

void ReelWidget::drawAUnit(const QRect & rect, QPainter * painter, uint number)
{
    painter->drawPixmap(rect,bg);
    painter->setPen(Qt::white);
    painter->drawText(rect,Qt::AlignCenter,QString::number(number));
}

testReelWidget::testReelWidget(QWidget *parent)
    :QWidget(parent)
{
    QHBoxLayout * hb = new QHBoxLayout(this);
    hb->setSpacing(0);

    setValue(123456789);
    connect(&timer,&QTimer::timeout,this,&testReelWidget::onTimer);
    timer.start(2000);
}

void testReelWidget::setValue(int value)
{
    QQueue<int> queue;
    while(value != 0)
    {
        queue.push_front(value % 10);
        value /= 10;
    }
    if(queue.size() != list.size())
    {
        auto layout = this->layout();
        for(auto w : list)
        {
            layout->removeWidget(w);
        }
        qDeleteAll(list);
        list.clear();
        for(auto value : queue)
        {
            auto w = new ReelWidget(this);
            layout->addWidget(w);
            list << w;
        }
    }
    for(int i = 0;i < list.size();++i)
    {
        list[i]->setNewNumber(queue.dequeue());
    }
}

QSize testReelWidget::sizeHint() const
{
    return QSize(list.size() * 55,90);
}


void testReelWidget::onTimer()
{
   QRandomGenerator m_generator  = QDateTime::currentMSecsSinceEpoch();


    for(auto w : list)
    {
        int randomNum = m_generator.bounded(10);
        w->setNewNumber(randomNum);
    }

}

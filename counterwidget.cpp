#include <QHBoxLayout>
#include "counterwidget.h"
#include "config.h"
#include "gpio.h"

CounterWidget::CounterWidget(QWidget *parent)
    : QWidget(parent), m_count(0), m_timer(this), m_gpio(this)
{
    setWindowTitle("BinaryCounter");
    resize(300, 200);
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    // QLCDNumber zur Anzeige des Zaehlerstandes dezimal:
       m_lcd = new QLCDNumber(2);
       layout->addWidget(m_lcd);
       m_lcd->display(m_num);



     connect(&m_timer, &QTimer::timeout, this, &CounterWidget::updateCounter);
    m_timer.start(T_UPDATE);
}

CounterWidget::~CounterWidget()
{


}

void CounterWidget::updateLEDs()
{
    for(int a = 0; a < 4; a++)
       m_gpio.set(LEDS[a], (m_num >> a) & 0b1);

}

// Zaehlerstand im Widget und dual ueber LEDs anzeigen:
void CounterWidget::updateCounter()
{
   if(m_gpio.isActivated(BUTTONS[2]))
       m_num= (m_num + 1) & 0xF;

   if(m_gpio.isActivated(BUTTONS[0]))
        m_num = (m_num -1) & 0xF;

   if(m_gpio.isActivated(BUTTONS[1]))
       m_num = 0;

   m_lcd->display(m_num);
   updateLEDs();

}



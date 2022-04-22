#include <lgpio.h>
#include "gpio.h"
#include "config.h"

Gpio::Gpio(QObject *parent) : QObject(parent)
{
    m_handle = lgGpiochipOpen(CHIP); // get a handle to the GPIO
    if (m_handle < 0)
        throw lguErrorText(m_handle);
    int init_level = 0;
    for (auto pin : LEDS) // Outputs
        lgGpioClaimOutput(m_handle, LFLAGS, pin, init_level);
    for (auto pin : BUTTONS) {// Inputs
        int error = lgGpioClaimInput(m_handle, LFLAGS, pin);
        if (error < 0)
            throw lguErrorText(error);
        m_oldstates.append(false);
    }
}

Gpio::~Gpio()

{
    // Cleanup GPIO
   int init_level = 0;
   for (auto pin : LEDS)
       lgGpioWrite(m_handle, pin, init_level);
   lgGpiochipClose(m_handle);




    }


// Write to pins
void Gpio::set(int pin, bool value)
{
    int result = lgGpioWrite(m_handle, pin, value);
    if(result < 0)
        throw lguErrorText(result);

}



// Read pin state
bool Gpio::get(int pin)
{
    int result = lgGpioRead(m_handle, pin);
    if (result < 0)
        throw lguErrorText(result);
    return result;
}

// Read pin state
bool Gpio::isActivated(int pin)
{
    int result = lgGpioRead(m_handle, pin);
    if (result < 0)
        throw lguErrorText(result);

    // rising slope detection
    bool rising = false;
    int index = BUTTONS.indexOf(pin);
    if (!result > m_oldstates[index]) // !result weil gedrueckt = LO
        rising = true;

    m_oldstates[index] = !result;
    return rising;
}

#include <stdint.h>
#include "inc/tm4c129xnczad.h"

int main(void)
{


    // Enable the GPIO port that is used for the on-board LED.
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R14;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    volatile uint32_t ui32Loop = SYSCTL_RCGCGPIO_R;

    // Enable the GPIO pin for the LED (PQ7).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIO_PORTQ_DIR_R = 0x80;
    GPIO_PORTQ_DEN_R = 0x80;


    while(1)
    {
        // Turn on the LED.
        GPIO_PORTQ_DATA_R |= 0x80;

        // Delay for a bit.
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }

        // Turn off the LED.
        GPIO_PORTQ_DATA_R &= ~(0x80);

        // Delay for a bit.
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }
}

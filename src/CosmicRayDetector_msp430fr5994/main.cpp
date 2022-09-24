#include <msp430.h>				

/**
 * Main entry point to the msp430 application
 */
void main(void)
{
    // stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;

	// configure P1.0 as output
	P1DIR |= 0x01;

	// clear the GPIO configuration lock bit (required on the FR part specifically.
	PM5CTL0 = PM5CTL0 & ~LOCKLPM5;

	// ensure the loop variable is not optimized away or cached...
	volatile unsigned int i = 0;

	while(1)
	{
	    // toggle P1.0
		P1OUT ^= 0x01;

		// delay for ... a while
		for(i=10000; i>0; i--);     // delay
	}
}

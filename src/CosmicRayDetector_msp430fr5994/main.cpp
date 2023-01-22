#include <msp430.h>				

void main(void)
{
    // stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;

	// configure P1.0 as output
	P1DIR |= 0x01;

	// clear the GPIO configuration lock bit (required on the FR part specifically.
	PM5CTL0 = PM5CTL0 & ~LOCKLPM5;



	// TODO: carve out redundant spots for statistics

	// TODO: carve out redundant copies of stack, program counter, stack pointer,
	//       special function registers etc.

	while(1)
	{
	    // toggle P1.0
		P1OUT ^= 0x01;

		// ensure the loop variable is not optimized away or cached...
		// delay for ... a while
		for(volatile unsigned int i = 10000; i > 0; i--);

		// check some memory for a bit

		// TODO: set wake-up timer and go to sleep

	}
}

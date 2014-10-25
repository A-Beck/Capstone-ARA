/*
 * This file demos IR functionality for the capstone
 */

#include <msp430.h> 

#define REDLED		BIT0
#define GREENLED	BIT6
#define BUTTON 		BIT3
#define FREQ		16
#define PERIOD		205


void port_setup(void);
void clock_setup(void);
void timer_setup(void);

void simple_demo(void);
void power_on(void);
void temp_up(void);
void temp_down(void);

volatile int flag = 0;

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    clock_setup();
	port_setup();
	timer_setup();
	_BIS_SR(GIE);			// enable interrupts

	while(1) {
//		if ((BUTTON & P1IN)){
//			_nop();
//		}
//		else {
//			simple_demo();
//		}
		simple_demo();
		int i = 0;
		for (i; i< FREQ; i++) {
			_delay_cycles(1e6);
		}
	}

	return 0;
}


void port_setup(void) {
	P1OUT &= ~REDLED;
	P1OUT &= ~GREENLED;
	P1DIR |= REDLED + GREENLED; 		// set up ports
	P1REN |= BUTTON;		 				//set up the button
}

void clock_setup() {
	BCSCTL1 = CALBC1_16MHZ;		// set frequency
	DCOCTL = CALDCO_16MHZ;		// set frequency
	BCSCTL2 |= SELM_0 + DIVM_0 + DIVS_0;
	/*
		 * source from the DCOCLK, which was previously set to 1MHZ
		 * divide MCLK frequency by 1
		 * divide SMCLK by 1
	*/
}

void timer_setup(void){
	TA0CCR0 = PERIOD;			// set the period
	TACCTL0 = CCIE | CM_0;		// enable capture
//	TA0CCR1 = 7;				// set starting value
//	TACCTL1 = CCIE | CM_0;	// enable capture
	TACTL = TASSEL_2 | ID_0 | TACLR | MC_1;
	/*
	* Source from the SMCLK
	* Don't divide frequency
    * clear the timer
    * count in up mode
	*/
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1(void) {	// when interrupt from A1 trips...

	_nop();
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {	// when interrupt from A0 trips

	TACCTL1 &= ~CCIFG;		// clear the flag
	if (flag==1) {
	P1OUT ^= GREENLED;		// toggle the green led
	}
}

void simple_demo(){

	flag = 1;
	P1OUT |= GREENLED;
	_delay_cycles(500*16);

	flag = 0;
	P1OUT &= ~GREENLED;
	_delay_cycles(500*16);

	flag = 1;
	P1OUT |= GREENLED;
	_delay_cycles(500*16);

	flag = 0;
	P1OUT &= ~GREENLED;
	_delay_cycles(500*16);
	_nop();

}

void power_on(void) {
	P1OUT &= ~GREENLED;

//	P1OUT ^= GREENLED;
//	flag = 1;
//	_delay_cycles(8474);

	P1OUT ^= GREENLED;
	flag ^= 1;
	_delay_cycles(4188);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(567);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(1539);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(572);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(1540);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(572);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(1537);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(573);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(484);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(572);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(1535);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(577);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(481);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(567);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(1540);

	P1OUT ^= GREENLED;
	_delay_cycles(572);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(1539);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(467);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(4152);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(578);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(1534);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(571);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(1540);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(570);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(487);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(572);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(1536);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(571);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(486);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(572);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(481);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(572);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(591);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(567);

	P1OUT ^= GREENLED;
	flag = 1;
	_delay_cycles(481);

	P1OUT ^= GREENLED;
	flag = 0;
	_delay_cycles(654);

	flag = 0;
	P1OUT &= ~GREENLED;
}

void temp_up(void){
	_nop();
}

void temp_down(void){
	_nop();
}

/*
 * This file demos IR functionality for the capstone
 */

#include <msp430.h> 

#define REDLED		BIT0  // port 1 bit 0
#define GREENLED	BIT7  // port 4 bit 7
#define BUTTON 		BIT1  // port 2 bit 1
#define FREQ		16    // freq in MHz
#define PERIOD		205

#define IRLEDPORT	P3OUT
#define IRLEDDIR  	P3DIR
#define IRLED		BIT7  // port 3 bit 7 --> may need to change

void port_setup(void);
void clock_setup(void);
void timer_setup(void);

void ir_transmit(int code[], int size);

// time in ms
int power_on_code[] = {
		5, 10, 7, 3
};

volatile int flag = 0;

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    clock_setup();
	port_setup();
	timer_setup();
	_BIS_SR(GIE);			// enable interrupts

	while(1) {
		if ((BUTTON & P1IN)){
			_nop();
		}

		else {
			ir_transmit(power_on_code, 4);
		}

		_delay_cycles(1e6);
		_delay_cycles(1e6);
		_delay_cycles(1e6);
		_delay_cycles(1e6);
		_delay_cycles(1e6);
		_delay_cycles(1e6);
		_delay_cycles(1e6);
		_delay_cycles(1e6);

	}

	return 0;
}


void port_setup(void) {

	// Set up the Various LED's
	P1OUT &= ~REDLED;
	P4OUT &= ~GREENLED;
	IRLEDPORT &= ~IRLED;
	P1DIR |= REDLED;
	P4DIR |= GREENLED;
	IRLEDDIR |= IRLED;

	//set up the button
	P2REN |= BUTTON;
}


void clock_setup() {
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
	UCSCTL2 = FLLD_1 + 486;                   // Set DCO Multiplier for 12MHz
	                                          // (N + 1) * FLLRef = Fdco
	                                          // (486+1) * 32768 = 16MHz
	                                          // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop


}


void timer_setup(void){
	TA0CCTL0 = CCIE | CM_0;		// enable capture
	TA0CCR0 = PERIOD;			// set the period
	TA0CTL = TASSEL_2 | ID_0 | TACLR | MC_1;  // source from SMLK, divide by 1, clear when hit limit, count up to CCR0
}

/*
 * code is an array which holds high/low time values in microseconds
 * first element is assumed high
 * size is length of the array
 */
void ir_transmit(int code[], int size) {
	volatile int i;
	flag = 0;
	for (i = 0; i < size; i++) {
		flag = 1-flag;  // flag toggle
		volatile int delay = code[i];

		if (flag == 1) {
			IRLEDPORT |= IRLED;
			_nop();
		}

		else if (flag == 0){
			IRLEDPORT &= ~IRLED;
			_nop();
		}

		volatile int j;
		for (j = 0; j < delay; j++) {
			_delay_cycles(1e3*FREQ);
		}

	}

	// turn off LED, stop transmission
	flag = 0;
	IRLEDPORT &= ~IRLED;
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {	// when interrupt from A0 trips

	TA0CCTL0 &= ~CCIFG;		     // clear the flag
	if (flag==1) {
	    IRLEDPORT ^= IRLED;		// toggle the green led
	}
}

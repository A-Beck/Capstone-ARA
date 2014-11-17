#include <msp430.h>

void WDTinit(void);		// sets the watchdog timer (turns it off)
void ADCinit(void);		// Initializes the ADC to take samples
void interruptInit(void);	// enables needed interrupts
void clockInit(void);	// initializes the clocks to correct source and frequency

void sample(void);
void convert(void);

// global variables
float temp = 0;
int data = 0;

int main(void)
{
  WDTinit();		// turn off the watchdog timer
  clockInit();	// initialize clocks to run at 1MHZ
  ADCinit();		// set up the ADC for sampling
  interruptInit();	// set up interrupts last, so setup isn't messed up


  while (1)
  {
    sample();
    convert();
    _nop();
    __delay_cycles(10000);
  }
}

void convert(void) {
	float millivolts = data * (3300.0/4096.0);  // 3.3 v is reference voltage, 12^2 = 4096 possible bins
	float celcius = (millivolts - 500.0) / 10.0;  // from data sheet of TMP36G
	temp = (celcius  * 1.8 + 32.0);  // basic c to f conversion
}

void sample(void) {
	ADC12CTL0 |= ADC12SC;                   // Start conversion
	while (!(ADC12IFG & BIT0));
	data = ADC12MEM0;		// get data from ADC reading
	__no_operation();                       // SET BREAKPOINT HERE


}

void WDTinit(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
}


void ADCinit(void){

	volatile unsigned int i;
	  P6SEL |= 0x01;                            // Enable A/D channel A0
	  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
	                                            // ADC12_A ref control registers
	  ADC12CTL0 = ADC12ON+ADC12SHT02+ADC12REFON;
	                                            // Turn on ADC12, Sampling time
	                                            // On Reference Generator and set to
	                                            // 2.5V
	  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
	  ADC12MCTL0 = ADC12SREF_0;                 // Vr+=Vref+ and Vr-=AVss

	  for ( i=0; i<0x30; i++);                  // Delay for reference start-up

	  ADC12CTL0 |= ADC12ENC;                    // Enable conversions
}

void interruptInit(void) {
	_BIS_SR(GIE);		// enable global interrupts
}

void clockInit(void) {
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
	UCSCTL2 = FLLD_1 + 486;                   // Set DCO Multiplier for 12MHz
	                                          // (N + 1) * FLLRef = Fdco
	                                          // (486+1) * 32768 = 16MHz
	                                          // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop
}


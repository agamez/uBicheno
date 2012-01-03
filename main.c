#include <htc.h>
#include <pic12f1822.h>

__CONFIG(MCLRE_OFF & CP_OFF & WDTE_OFF & CPD_OFF & BOREN_OFF & FOSC_INTOSC);

#define _XTAL_FREQ 31000

#define START_PWM() do {TMR2ON = 1;} while(0)
#define STOP_PWM()  do {TMR2ON = 0;} while(0)

void setupPWM()
{
        /* Disable the CCP1 pin output driver by setting the associated TRIS bit. */
        TRISA = 0b100000;
        /* Load the PR2 register with the PWM period value. */
        PR2 = 2;

        /* Configure the CCP1 module for the PWM mode by loading
         * the CCP1CON registerwith the appropriate values.
         */
        CCP1CON = 0b00001100;

        /* Load the CCPR1L register and the DC1B1 bits of
         * the CCP1CON register, with the PWM duty cycle value.
         */
        CCPR1L = 0;
        DC1B1 = 1;
        DC1B0 = 0;

        /* Configure and start Timer2: */
        TMR2IF = 0;
        /* Configure the T2CKPS bits of the T2CON register with the Timer prescale value. */
        T2CKPS1 = 0;
        T2CKPS0 = 0;
        /* Enable the CCP1 pin output driver by clearing the associated TRIS bit. */
        TRISA = 0b000000;

        /* Enable the Timer by setting the TMR2ON bit of the T2CON register. */
}

unsigned char rand_between(unsigned char min, unsigned char max)
{
        return rand() % (max-min) + min;
}


void main(void)
{
        unsigned char n_rand;
        unsigned char i, n, j;

        OSCCON = 0b00000011;

        /* For 8 Pin Devices (PIC12F/LF1822):
         *  0 = CCP1/P1A function is on RA2
         *  1 = CCP1/P1A function is on RA5
         */
        CCP1SEL = 1;
        setupPWM();

	/* Initial short beep: ready */
	__delay_ms(500);
        START_PWM();
        __delay_ms(250);
        STOP_PWM();

        /* Wait for about 10 minutes before the loop starts so we can run away from the location */
	n=10;
        while(n--) {
                __delay_ms(60000);
        }

        for (;;) {
                i = rand_between(3, 5);
                while(i--) {
                        START_PWM();
                        n = rand_between(2, 5);
                        while(n--) {
                                __delay_ms(1000);
                        }
                        STOP_PWM();
                        n = rand_between(5, 9);
                        while(n--) {
                                __delay_ms(1000);
                        }
                }
                n = rand_between(5, 20);
                while(n--) {
                        __delay_ms(60000);
                }
        }
}

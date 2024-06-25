#include <p18cxxx.h>
#pragma config FOSC=INTIO67
#pragma config WDTEN=OFF
#pragma config LVP=OFF

int area;
int duration;
float input;
unsigned int delayVal=25000;
unsigned int number;

unsigned char SwitchPressed(volatile near unsigned char *sw, char bitnum);
void delay (void);
void leggimoneta (void);
void riconoscimoneta(void);
void display(void);

void main(void) {
    TRISA=0xFF;                             // enabling PORTA as INPUT
    TRISB=0x00;                             // enabling PORTB as OUTPUT
    TRISC=0x00;                             // enabling PORTC as OUTPUT
    ANSELA=0x01;                            // enabling RA0 as analogical
    ADCON2=0x38;                            // set up conversion time=4TAD and clock conversion=FOSC/4
    ADCON0bits.CHS0=0;                      // select AN0 as the channel to convert
    ADCON0bits.CHS1=0;
    ADCON0bits.CHS2=0;
    ADCON0bits.CHS3=0;
    ADCON1bits.PVCFG0=0;                    // VDD and VSS as the reference
    ADCON1bits.PVCFG1=0;
    ADCON1bits.NVCFG0=0;
    ADCON1bits.NVCFG1=0;
    ADCON0bits.ADON=1;                      // ADC enabling

    while(1) {
        PORTCbits.RC0=0;                    // green LED off
        PORTCbits.RC1=1;                    // red LED on
        while(!SwitchPressed(&PORTA, 3)) {
            delay();
            PORTCbits.RC0=1;                // green LED on 
            PORTCbits.RC1=0;                // red LED off 
        }
    }
    
    delay();

    do {
        while(PORTAbits.RA1==0);            // coin entrance
        while(PORTAbits.RA1==1);
        PORTCbits.RC0=0;                    // green LED off
        PORTCbits.RC1=1;                    // red LED on
        leggimoneta();                      // passing of the coin through the sensor
        riconoscimoneta();                  // recognition of the coin 
        duratation=0;
        PORTCbits.RC0=1;                    // green LED on
        PORTCbits.RC1=0;                    // red LED off
        delay();
    }
}

void leggimoneta(void) {
    area=0;
    while(PORTAbits.RA2==0) {
        ADCON0bits.GO=1;
        while(ADCON0bits.GO);
        input=ADRESH;
        if(input>65) {                      //acquisition if the input signal is higher than 0,8 V
            area=area+(input*19,6)/1000;
            duratation++;
        }
    }
}

void riconoscimoneta(void) {
    if (((duration>=10)&&(duration<=17))&&((area>=0)&&(area<=49))) {
        number=1;
    }
    if (((duration>=18)&&(duration<=24))&&((area>=50)&&(area<=79))) {
        number=7;
    }
    if (((duration>=25)&&(duration<=255))&&((area>=80)&&(area<=255))) {
        number=8;
    }
    display();
}

void display(void) {
    switch(number) {
        case 0: PORTB=0x00; break;
        case 1: PORTB=0x4B; break;
        case 2: PORTB=0x3D; break;
        case 3: PORTB=0x6D; break;
        case 4: PORTB=0x48; break;
        case 5: PORTB=0x67; break;
        case 6: PORTB=0x73; break;
        case 7: PORTB=0x4C; break;
        case 8: PORTB=0xFF; break;
        case 9: PORTB=0x4F; break;
    }
}

void delay(void) {
    unsigned int i=0;
    for (i=0; i<delayVal; i++);
}


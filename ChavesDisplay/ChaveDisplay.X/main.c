
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#define _XTAL_FREQ 4000000

#define chave_mais  !PORTAbits.RA2
#define chave_menos  !PORTAbits.RA1

int display[10] = {0xee, 0x28, 0xcd, 0x6d, 0x2b, 0x67, 0xe7, 0x2c, 0xef, 0x2f};


void configuracao() 
{
    TRISA = 0xff;
    TRISB = 0x00;
}

void main(void) 
{
    int numero = 0;
    configuracao();
    
    for (;;) 
    {
        PORTB = display[numero];
        
        if (chave_mais)
        {
            if (!PORTAbits.RA2)
            {
                __delay_ms(500);
            }
            numero++;
            if (numero > 9)
                numero = 9;
        }
        
        if (chave_menos)
        {
            if (!PORTAbits.RA1)
                __delay_ms(500);
            numero--;
            if (numero < 0)
                numero = 0;
        }
    }
    
    return;
}

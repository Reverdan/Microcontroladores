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
#define chaveMenos !PORTAbits.RA1
#define chaveMais !PORTAbits.RA2


int display[10] = {0xee, 0x28, 0xcd, 0x6d, 0x2b, 0x67, 0xe7, 0x2c, 0xef, 0x6f};
unsigned int numero = 0;

void mostrar();

void __interrupt() interrupcao(void)
{
    if (TMR0IE && TMR0IF)
    {
        TMR0IF = 0;
        mostrar();
    }
    return;
}

unsigned char eeprom_leitura(unsigned char endereco)
{
    EEADR = endereco; // Registrador EEADR armazena o endereço da EEPROM a ser acessado
    EECON1bits.RD = 1; // Configura o bit RD (Read) como 1 para iniciar a operação de leitura
    // 4. A leitura é realizada em um ciclo de máquina. O dado fica imediatamente disponível no registrador EEDATA
    return EEDATA; // Lê o byte do Registrador EEDATA, onde o valor da EEPROM é colocado após a leitura
}

void eeprom_escrita(unsigned char endereco, unsigned char valor)
{
    while(EECON1bits.WR); // Aguarda até que uma operação de escrita anterior seja concluída

    EEADR = endereco;  // Carrega o endereço
    EEDATA = valor;    // Carrega o dado a ser escrito

    EECON1bits.WREN = 1;  // Habilita operações de escrita
    INTCONbits.GIE = 0;   // Desabilita interrupções globalmente

    // Executa a sequência de segurança obrigatória
    EECON2 = 0x55;
    EECON2 = 0xAA;

    EECON1bits.WR = 1;    // Inicia o ciclo de escrita
    INTCONbits.GIE = 1;   // Reabilita interrupções

    while(EECON1bits.WR); // Aguarda a conclusão do ciclo de escrita
    EECON1bits.WREN = 0;  // Desabilita futuras escritas
}

void configuracao()
{
    TRISB = 0x00;
    PORTB = 0x00;
    
    TMR0 = 0x00;
    OPTION_REG = 0x06;
    INTCON = 0xA0;
    
    numero = eeprom_leitura(0x00);
}

void mostrar()
{
    int unidade = 0, dezena = 0;
    dezena = numero / 10;
    unidade = numero - dezena * 10;
    if (PORTBbits.RB4) 
    {
        PORTB = display[dezena];
        PORTBbits.RB4 = 0;
    }
    else 
    {
        PORTB = display[unidade];
        PORTBbits.RB4 = 1;
    }
}


void main(void)
{
    configuracao();
    for(;;)
    {
        if (chaveMais)
        {
            numero++;
            if (numero > 99) numero = 0;
            eeprom_escrita(0x00, numero);
            __delay_ms(300);
        }
        if (chaveMenos)
        {
            numero--;
            if (numero > 99) numero = 99;
            eeprom_escrita(0x00, numero);
            __delay_ms(300);
        }
    }

}


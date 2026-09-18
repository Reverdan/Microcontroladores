# README — Explicação do `main.c` (Projeto Display)

Este projeto configura um microcontrolador PIC para usar a **porta B como saída** e manter, continuamente, um padrão lógico fixo em `PORTB`.

## Arquivo analisado

- `/home/runner/work/Microcontroladores/Microcontroladores/Display/Display.X/main.c`

## Objetivo do código

O programa inicializa o PIC e, em loop infinito, escreve `0x7D` em `PORTB`.  
Na prática, isso mantém um mesmo estado elétrico nos pinos RB0–RB7 (por exemplo, para acionar um display de 7 segmentos ou LEDs, dependendo do circuito físico).

## Estrutura do código

### 1) Bits de configuração (`#pragma config`)

No início do arquivo, os `#pragma config` definem como o PIC deve iniciar:

- `FOSC = HS`: usa oscilador de alta velocidade (cristal/resonador externo).
- `WDTE = OFF`: desativa watchdog timer.
- `PWRTE = OFF`: desativa power-up timer.
- `MCLRE = ON`: pino MCLR habilitado para reset externo.
- `BOREN = OFF`: desativa brown-out reset.
- `LVP = OFF`: desativa gravação em baixa tensão.
- `CPD = OFF` e `CP = OFF`: desativa proteção de código/dados.

Essas opções afetam o comportamento de hardware antes mesmo de `main()` rodar.

### 2) Inclusões e frequência de clock

- `#include <xc.h>`: cabeçalho principal da Microchip (registradores e definições do PIC).
- `#define _XTAL_FREQ 4000000`: informa clock de 4 MHz para funções de delay do XC8 (mesmo que não haja `__delay_ms()` neste arquivo).

### 3) Função `configuracao()`

```c
void configuracao()
{
    TRISB = 0x00;
}
```

- `TRISB` define direção dos pinos da porta B.
- `0` em cada bit significa **saída**.
- `0x00` deixa **RB0 a RB7 como saídas digitais**.

### 4) Função `main()`

```c
void main(void) 
{
    configuracao();
    
    for(;;)
    {
        PORTB = 0x7D;
    }
    
    return;
}
```

Fluxo:

1. Chama `configuracao()` para preparar a porta B.
2. Entra em `for(;;)` (loop infinito).
3. Escreve continuamente `0x7D` em `PORTB`.

## O que significa `PORTB = 0x7D`

- `0x7D` em hexadecimal = `01111101` em binário.
- Isso define níveis lógicos fixos nos pinos RB7..RB0.

Mapeando bit a bit:

- RB7 = 0
- RB6 = 1
- RB5 = 1
- RB4 = 1
- RB3 = 1
- RB2 = 1
- RB1 = 0
- RB0 = 1

O efeito visual final depende da ligação elétrica:

- tipo do display (ânodo comum ou cátodo comum),
- resistores,
- ordem de conexão dos segmentos aos pinos RBx.

## Observações importantes

- O `return;` ao final de `main` não é alcançado, porque o laço é infinito.
- O código é intencionalmente simples para demonstrar controle direto de portas.
- Se o objetivo for exibir outros dígitos/padrões, basta alterar o valor enviado para `PORTB`.

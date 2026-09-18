# Projeto Pisca

Este projeto mostra como controlar **8 LEDs ligados ao `PORTB`** de um microcontrolador PIC, fazendo um efeito de pisca alternado entre os bits pares e ímpares da porta.

O arquivo-fonte principal do exemplo é `/home/runner/work/Microcontroladores/Microcontroladores/Pisca/led.c`. Embora o pedido mencione `main.c`, neste projeto a lógica principal está nesse arquivo, dentro da função `main`.

## Objetivo do programa

O programa acende e apaga os LEDs em dois padrões:

- `0xAA` → `10101010`
- `0x55` → `01010101`

Com isso, os LEDs alternam entre dois grupos, criando um efeito visual de pisca.

## Estrutura do código

### 1. Bits de configuração

No início do arquivo aparecem várias diretivas `#pragma config`:

- `FOSC = HS` define oscilador de alta velocidade
- `WDTE = OFF` desativa o watchdog timer
- `PWRTE = OFF` desativa o power-up timer
- `MCLRE = ON` mantém o pino MCLR habilitado
- `BOREN = OFF` desativa brown-out reset
- `LVP = OFF` desativa gravação em baixa tensão
- `CPD = OFF` e `CP = OFF` desativam proteção de código

Essas configurações preparam o funcionamento básico do PIC antes da execução do programa.

### 2. Biblioteca usada

```c
#include <xc.h>
```

Essa biblioteca dá acesso aos registradores do microcontrolador, como `TRISB` e `PORTB`.

### 3. Frequência do cristal

```c
#define _XTAL_FREQ 4000000
```

Define a frequência de trabalho em **4 MHz**. Isso é importante para que a função `__delay_ms()` gere atrasos corretos.

### 4. Função de configuração

```c
void configuracao()
{
    TRISB = 0x00;
}
```

`TRISB` define se cada pino da porta B será entrada ou saída:

- bit `1` → entrada
- bit `0` → saída

Quando o código faz:

```c
TRISB = 0x00;
```

todos os 8 bits do `PORTB` ficam como **saída**. Isso é necessário porque os LEDs precisam receber sinais do microcontrolador.

## Função principal

```c
void main(void) 
{
    configuracao();
    
    for(;;)
    {
        PORTB = 0xAA;
        __delay_ms(500);
        PORTB = 0x55;
        __delay_ms(500);
    }
    
    return;
}
```

### Passo a passo

1. `configuracao();` coloca o `PORTB` como saída.
2. `for(;;)` cria um laço infinito.
3. `PORTB = 0xAA;` envia o padrão `10101010` para os 8 LEDs.
4. `__delay_ms(500);` espera 500 ms.
5. `PORTB = 0x55;` envia o padrão `01010101`.
6. `__delay_ms(500);` espera mais 500 ms.
7. O ciclo recomeça indefinidamente.

## Entendendo os valores hexadecimais

### Padrão `0xAA`

`0xAA` em binário é:

```text
10101010
```

Isso significa:

- RB7 = 1
- RB6 = 0
- RB5 = 1
- RB4 = 0
- RB3 = 1
- RB2 = 0
- RB1 = 1
- RB0 = 0

Ou seja, metade dos LEDs fica em um estado e a outra metade no estado oposto.

### Padrão `0x55`

`0x55` em binário é:

```text
01010101
```

Agora o padrão se inverte:

- RB7 = 0
- RB6 = 1
- RB5 = 0
- RB4 = 1
- RB3 = 0
- RB2 = 1
- RB1 = 0
- RB0 = 1

Assim, os LEDs que estavam apagados passam a acender, e os que estavam acesos passam a apagar.

## Tabela dos padrões

| Valor enviado ao PORTB | Binário    | Efeito nos 8 LEDs |
|---|---|---|
| `0xAA` | `10101010` | LEDs alternados em uma configuração |
| `0x55` | `01010101` | LEDs alternados na configuração inversa |

## Efeito visual

Se os LEDs estiverem ligados diretamente aos pinos `RB0` até `RB7`, o observador verá:

- um grupo de LEDs aceso
- meio segundo depois, o grupo oposto aceso
- esse processo se repetindo continuamente

O resultado é um **pisca alternado**.

## Resumo didático

Este exemplo ensina quatro ideias centrais:

1. como configurar uma porta do PIC como saída
2. como escrever valores diretamente em um registrador de porta
3. como usar padrões binários para controlar vários LEDs ao mesmo tempo
4. como criar animação simples com atrasos

Em resumo, o programa transforma o `PORTB` em uma saída de 8 bits e alterna entre `0xAA` e `0x55` a cada 500 ms para produzir o efeito de pisca nos 8 LEDs.

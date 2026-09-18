# README — Explicação do `main.c` (Projeto Semáforo)

Este projeto simula um **semáforo simples** usando LEDs ligados ao `PORTB` de um microcontrolador PIC.

## Arquivo analisado

- `/home/runner/work/Microcontroladores/Microcontroladores/Semaforo/Semaforo.X/main.c`

## Objetivo do código

Inicializar a porta B como saída e repetir, em loop infinito, a sequência:

1. LED verde aceso por 2,5 s
2. LED amarelo aceso por 0,5 s
3. LED vermelho aceso por 1,5 s

## Estrutura do programa

### 1) Bits de configuração (`#pragma config`)

As diretivas no topo do arquivo definem o comportamento de hardware do PIC:

- `FOSC = HS`: oscilador de alta velocidade (cristal/resonador externo).
- `WDTE = OFF`: watchdog desativado.
- `PWRTE = OFF`: power-up timer desativado.
- `MCLRE = ON`: pino MCLR habilitado para reset externo.
- `BOREN = OFF`: brown-out reset desativado.
- `LVP = OFF`: gravação em baixa tensão desativada.
- `CPD = OFF` e `CP = OFF`: proteção de memória/código desativada.

Essas configurações são aplicadas antes da execução de `main()`.

### 2) Biblioteca e clock

- `#include <xc.h>`: acesso aos registradores do PIC.
- `#define _XTAL_FREQ 4000000`: informa clock de 4 MHz para que `__delay_ms()` gere atrasos corretos.

### 3) Macros de estado e tempo

O código define atalhos para tornar a lógica mais legível:

- `verde` → `PORTB = 0x04`
- `amarelo` → `PORTB = 0x02`
- `vermelho` → `PORTB = 0x01`
- `tempo_verde` → `__delay_ms(2500)`
- `tempo_amarelo` → `__delay_ms(500)`
- `tempo_vermelho` → `__delay_ms(1500)`

Assim, cada macro representa uma ação de semáforo (cor + duração).

### 4) Função `configuracao()`

```c
void configuracao()
{
    TRISB = 0x00;
}
```

- `TRISB` define direção dos pinos da porta B.
- `0x00` significa que **RB0 a RB7 são saídas**.
- Isso é necessário para acionar LEDs.

### 5) Função `main()`

```c
void main(void) 
{
    configuracao();
    
    for(;;)
    {
        verde;
        tempo_verde;
        amarelo;
        tempo_amarelo;
        vermelho;
        tempo_vermelho;
    }
    
    return;
}
```

Fluxo:

1. Configura `PORTB` como saída.
2. Entra em laço infinito (`for(;;)`).
3. Escreve o padrão da cor atual em `PORTB`.
4. Aguarda o tempo correspondente.
5. Repete a sequência continuamente.

## Entendendo os valores em `PORTB`

### `PORTB = 0x04` (verde)

- Binário: `00000100`
- Bit em nível alto: RB2

### `PORTB = 0x02` (amarelo)

- Binário: `00000010`
- Bit em nível alto: RB1

### `PORTB = 0x01` (vermelho)

- Binário: `00000001`
- Bit em nível alto: RB0

> O LED que acende em cada etapa depende da ligação física dos LEDs aos pinos RB0, RB1 e RB2, além do tipo de montagem elétrica (ânodo/cátodo comum e resistores).

## Tempo total de um ciclo

Somando os atrasos:

- Verde: 2500 ms
- Amarelo: 500 ms
- Vermelho: 1500 ms

**Ciclo completo = 4500 ms (4,5 s)**.

## Resumo didático

Este código ensina os pontos centrais de controle digital em PIC:

1. configurar direção de pinos (`TRISB`);
2. escrever padrões binários em uma porta (`PORTB`);
3. controlar sequência temporal com `__delay_ms()`;
4. implementar comportamento contínuo com loop infinito.

Em resumo, é uma simulação direta de semáforo em hardware, com três estados e tempos definidos.

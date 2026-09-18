# README — Explicação do `main.c` (Projeto Semáforo 2 Ruas)

Este projeto implementa, em PIC + XC8, a lógica de um **cruzamento com duas ruas**.  
O controle é feito escrevendo padrões binários em `PORTB` com tempos definidos por `__delay_ms()`.

## Arquivo analisado

- `/home/runner/work/Microcontroladores/Microcontroladores/Semaforo2Ruas/Semaforo2Ruas.X/main.c`

## Objetivo do código

Executar continuamente a sequência de estados de um semáforo de cruzamento:

1. transição de segurança (todos os vermelhos),
2. rua A abre (verde), depois atenção (amarelo),
3. nova transição de segurança,
4. rua B abre (verde), depois atenção (amarelo),
5. reinicia o ciclo.

---

## Estrutura do programa

### 1) Bits de configuração (`#pragma config`)

As diretivas do início (`FOSC`, `WDTE`, `MCLRE`, etc.) definem o comportamento de hardware do PIC antes do `main()`.

### 2) Biblioteca e clock

- `#include <xc.h>`: acesso aos registradores do microcontrolador.
- `#define _XTAL_FREQ 4000000`: informa clock de 4 MHz para cálculo correto dos delays.

### 3) Função `configuracao()`

`TRISB = 0x00` coloca todos os pinos de `RB0..RB7` como **saída**, permitindo acionar LEDs.

### 4) Laço principal (`for(;;)`)

No laço infinito, o programa escreve em `PORTB`:

- `0x21` por 500 ms
- `0x81` por 1500 ms
- `0x41` por 500 ms
- `0x21` por 500 ms
- `0x24` por 2500 ms
- `0x22` por 500 ms

Depois, repete desde o início.

---

## Lógica do cruzamento (ponto principal)

A lógica é de **alternância entre as duas ruas**, com estágio de segurança entre elas:

- Quando uma rua está em **verde** (ou **amarelo**), a outra fica em **vermelho**.
- Antes de trocar a preferência da via, o código entra em um estado curto de transição (`0x21`) para reduzir risco de conflito no cruzamento.
- O mesmo padrão se repete indefinidamente, formando um controlador cíclico de estados.

Em outras palavras, o programa é uma **máquina de estados temporizada**:  
cada escrita em `PORTB` define o estado atual, e cada `__delay_ms()` define por quanto tempo esse estado permanece.

---

## Tabela de estados em função do tempo

> Referência temporal considerando `t = 0` no início do ciclo.

| Intervalo de tempo no ciclo | `PORTB` | Binário (`RB7..RB0`) | Bits em nível alto | Interpretação lógica do cruzamento |
|---|---:|---|---|---|
| `0,0 s` até `0,5 s` | `0x21` | `00100001` | RB5, RB0 | Transição de segurança (todos os vermelhos) |
| `0,5 s` até `2,0 s` | `0x81` | `10000001` | RB7, RB0 | Rua A verde, Rua B vermelha |
| `2,0 s` até `2,5 s` | `0x41` | `01000001` | RB6, RB0 | Rua A amarela, Rua B vermelha |
| `2,5 s` até `3,0 s` | `0x21` | `00100001` | RB5, RB0 | Nova transição de segurança |
| `3,0 s` até `5,5 s` | `0x24` | `00100100` | RB5, RB2 | Rua A vermelha, Rua B verde |
| `5,5 s` até `6,0 s` | `0x22` | `00100010` | RB5, RB1 | Rua A vermelha, Rua B amarela |

**Tempo total do ciclo: 6,0 s.**

Após `6,0 s`, o ciclo retorna ao primeiro estado (`0x21`) e continua.

---

## Mapeamento prático dos LEDs

O código trabalha com bits de `PORTB`; o significado físico exato (qual bit é verde/amarelo/vermelho de cada rua) depende da montagem elétrica da placa.  
Pela sequência usada, a interpretação lógica do exercício é:

- Rua A usa os estados associados a RB7 (verde) e RB6 (amarelo), com oposição em vermelho.
- Rua B usa os estados associados a RB2 (verde) e RB1 (amarelo), com oposição em vermelho.
- `0x21` aparece como estado de transição/sincronismo entre fases.

## Resumo didático

Este `main.c` demonstra três ideias fundamentais:

1. **Saída digital**: configurar `TRISB`.
2. **Estado lógico**: escrever padrões em `PORTB`.
3. **Controle temporal**: usar delays para manter cada estado pelo tempo certo.

Com isso, o programa realiza um semáforo de duas ruas por alternância cíclica e segura de estados.

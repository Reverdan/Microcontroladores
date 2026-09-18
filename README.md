# Microcontroladores

Este repositório reúne exemplos simples de programação para microcontroladores PIC usando XC8.  
O objetivo deste README é explicar, de forma didática, o que cada arquivo `main.c` faz e como o programa conversa com os pinos do microcontrolador.

## Visão geral

Os projetos principais do repositório são:

- `/home/runner/work/Microcontroladores/Microcontroladores/Semaforo/Semaforo.X/main.c`
- `/home/runner/work/Microcontroladores/Microcontroladores/Display/Display.X/main.c`
- `/home/runner/work/Microcontroladores/Microcontroladores/Semaforo2Ruas/Semaforo2Ruas.X/main.c`

Todos seguem a mesma ideia básica:

1. Configurar o microcontrolador.
2. Definir a direção dos pinos.
3. Escrever valores em `PORTB`.
4. Repetir esse comportamento para sempre dentro de um laço infinito.

---

## Conceitos importantes antes de ler os arquivos

### `#pragma config`

As linhas `#pragma config` configuram características de hardware do PIC, como:

- tipo de oscilador;
- watchdog timer;
- brown-out;
- proteção de código;
- modo de gravação.

Essas opções são gravadas como configuração do microcontrolador e não fazem parte da lógica principal do programa, mas são essenciais para o projeto funcionar corretamente.

### `#include <xc.h>`

Essa biblioteca dá acesso aos registradores e recursos do microcontrolador, como:

- `TRISB`
- `PORTB`
- `__delay_ms()`

Sem ela, o compilador não saberia como acessar o hardware.

### `_XTAL_FREQ`

```c
#define _XTAL_FREQ 4000000
```

Essa definição informa ao compilador que o clock do microcontrolador é de **4 MHz**.  
Isso é importante porque funções como `__delay_ms()` dependem dessa informação para calcular o tempo corretamente.

### `TRISB`

`TRISB` define se cada pino da porta B será entrada ou saída:

- `0` = saída
- `1` = entrada

Quando o código usa:

```c
TRISB = 0x00;
```

significa que **todos os pinos da porta B foram configurados como saída**.

### `PORTB`

`PORTB` é o registrador usado para colocar nível lógico nos pinos da porta B.  
Ao escrever um valor nele, o programa liga e desliga pinos específicos.

Exemplo:

```c
PORTB = 0x04;
```

Esse valor em binário é `00000100`, então apenas o bit correspondente fica ligado.

### `for(;;)`

Esse comando cria um **laço infinito**:

```c
for(;;)
{
}
```

Em sistemas embarcados isso é comum, porque o microcontrolador deve continuar executando a tarefa sem parar enquanto estiver energizado.

---

## 1. Projeto Semáforo

Arquivo: `/home/runner/work/Microcontroladores/Microcontroladores/Semaforo/Semaforo.X/main.c`

### O que esse programa faz

Simula um semáforo simples com três estados:

- verde;
- amarelo;
- vermelho.

Cada estado escreve um valor diferente em `PORTB` e espera um tempo antes de passar para o próximo.

### Estrutura do código

#### Macros de cor

```c
#define verde  PORTB = 0x04
#define amarelo PORTB = 0x02
#define vermelho PORTB = 0x01
```

Essas macros funcionam como atalhos:

- `verde` coloca `0x04` em `PORTB`;
- `amarelo` coloca `0x02` em `PORTB`;
- `vermelho` coloca `0x01` em `PORTB`.

Na prática, cada valor liga um pino diferente da porta B.  
Se LEDs estiverem conectados nesses pinos, um LED acende por vez.

#### Macros de tempo

```c
#define tempo_verde __delay_ms(2500)
#define tempo_amarelo __delay_ms(500)
#define tempo_vermelho __delay_ms(1500)
```

Essas macros representam o tempo que cada cor permanece acesa:

- verde por 2500 ms;
- amarelo por 500 ms;
- vermelho por 1500 ms.

#### Função `configuracao()`

```c
void configuracao()
{
    TRISB = 0x00;
}
```

Essa função prepara a porta B para uso como saída.  
Sem isso, os LEDs ou sinais conectados poderiam não responder como esperado.

#### Função `main()`

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
}
```

Passo a passo:

1. chama `configuracao()` para definir a porta B como saída;
2. entra em um laço infinito;
3. acende o verde;
4. espera;
5. acende o amarelo;
6. espera;
7. acende o vermelho;
8. espera;
9. recomeça do início.

### Resumo didático

Esse arquivo mostra bem a lógica básica de sistemas embarcados:

- configurar hardware;
- escrever nos pinos;
- esperar;
- repetir.

---

## 2. Projeto Display

Arquivo: `/home/runner/work/Microcontroladores/Microcontroladores/Display/Display.X/main.c`

### O que esse programa faz

Esse programa escreve continuamente um único valor em `PORTB`:

```c
PORTB = 0x7D;
```

Isso sugere o acionamento fixo de um display de 7 segmentos ou outro circuito conectado à porta B.

### Estrutura do código

#### Função `configuracao()`

Assim como no projeto anterior:

```c
void configuracao()
{
    TRISB = 0x00;
}
```

Todos os pinos da porta B são configurados como saída.

#### Função `main()`

```c
void main(void)
{
    configuracao();

    for(;;)
    {
        PORTB = 0x7D;
    }
}
```

Passo a passo:

1. configura a porta B como saída;
2. entra em laço infinito;
3. escreve sempre `0x7D` em `PORTB`.

### O que significa `0x7D`

`0x7D` em hexadecimal corresponde a:

```text
01111101
```

Cada bit controla um pino da porta B.  
Dependendo de como o display ou circuito foi montado, esse valor pode representar um número específico ou um padrão visual fixo.

### Resumo didático

Esse `main.c` é um bom exemplo de programa com saída constante:

- não há troca de estados;
- não há atrasos;
- o microcontrolador mantém o mesmo padrão elétrico continuamente.

---

## 3. Projeto Semáforo de 2 ruas

Arquivo: `/home/runner/work/Microcontroladores/Microcontroladores/Semaforo2Ruas/Semaforo2Ruas.X/main.c`

### O que esse programa faz

Esse projeto simula um semáforo mais completo, controlando duas vias.  
Por isso aparecem vários valores diferentes escritos em `PORTB` e vários tempos de espera.

### Estrutura do código

#### Função `configuracao()`

```c
void configuracao()
{
    TRISB = 0x00;
}
```

Novamente, todos os pinos da porta B são saídas.

#### Função `main()`

Dentro do laço infinito, o programa executa a sequência:

```c
PORTB = 0x21;
__delay_ms(500);
PORTB = 0x81;
__delay_ms(1500);
PORTB = 0x41;
__delay_ms(500);
PORTB = 0x21;
__delay_ms(500);
PORTB = 0x24;
__delay_ms(2500);
PORTB = 0x22;
__delay_ms(500);
```

### Como interpretar

Cada valor hexadecimal liga uma combinação diferente de bits da porta B.  
Como o nome do projeto é `Semaforo2Ruas`, a ideia é que esses bits controlem LEDs de dois conjuntos de semáforo ao mesmo tempo.

Assim, o programa provavelmente alterna entre estados como:

- uma rua liberada e outra fechada;
- transição com amarelo;
- troca de prioridade entre as ruas.

### Leitura didática da sequência

Mesmo sem um diagrama elétrico, a lógica pode ser entendida assim:

1. o programa liga uma combinação inicial de LEDs;
2. espera 500 ms;
3. muda para outra combinação;
4. espera 1500 ms;
5. faz nova transição;
6. espera;
7. libera a outra via;
8. espera mais tempo;
9. entra em transição novamente;
10. reinicia o ciclo.

### Resumo didático

Esse exemplo ensina que:

- um único registrador pode controlar vários sinais ao mesmo tempo;
- diferentes combinações de bits representam diferentes estados do sistema;
- atrasos determinam quanto tempo cada estado permanece ativo.

---

## Comparando os três `main.c`

### Semáforo simples

- alterna 3 estados;
- usa macros para deixar a leitura mais amigável;
- ideal para aprender sequência básica de controle.

### Display

- mantém um único valor constante na saída;
- mostra o uso mais simples possível de `PORTB`;
- ideal para entender escrita direta em porta.

### Semáforo de 2 ruas

- alterna várias combinações de bits;
- controla mais de um sinal ao mesmo tempo;
- ideal para entender sistemas com múltiplos estados.

---

## Conclusão

Os arquivos `main.c` deste repositório são exemplos introdutórios de programação embarcada com PIC.  
Eles mostram, de forma prática, três ideias fundamentais:

1. configurar o hardware;
2. definir saídas digitais;
3. controlar dispositivos externos escrevendo valores nos registradores.

Se você está começando, a melhor ordem de estudo é:

1. `Display`, para entender `PORTB`;
2. `Semaforo`, para entender sequência com atrasos;
3. `Semaforo2Ruas`, para entender combinações de bits e estados mais complexos.

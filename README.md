# Microcontroladores

Repositório com exercícios práticos de microcontroladores PIC, focados em controle digital usando `PORTB`, `TRISB` e temporização com XC8.

## Exercícios

### 1. [Pisca](./Pisca/README.md)
Exercício introdutório que alterna dois padrões binários (`0xAA` e `0x55`) no `PORTB` para criar um efeito de LEDs piscando.

### 2. [Display](./Display/Display.X/README.md)
Exemplo que mantém um padrão fixo (`0x7D`) em `PORTB`, útil para estudo de acionamento de display de 7 segmentos ou LEDs.

### 3. [Semáforo](./Semaforo/Semaforo.X/README.md)
Projeto que simula um semáforo simples com três estados principais: verde, amarelo e vermelho, cada um com seu tempo definido.

### 4. [Semáforo 2 Ruas](./Semaforo2Ruas/Semaforo2Ruas.X/README.md)
Exercício que implementa a lógica de um cruzamento com duas ruas, alternando os estados dos semáforos com transições de segurança.
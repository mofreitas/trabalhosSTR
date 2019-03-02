# trabalhosSTR
Trabalhos da disciplina de STR

Integrantes
- Felipe Lins
- Luis Henrique
- Matheus Oliveira

## Trabalho 1

Criar um programa na *Beaglebone Black* que cumpra os seguintes requisitos:

- Escrita em Pino Digital
- Leitura em Pino Digital
- Uso de Pino PWM
- Uso de Pino ADC
- Log do estados de cada pino

Dessa forma, criamos um programa que lê a tensão sobre um potenciômetro emite o sinal proporcional por meio de um PWM a um LED. Seu funcionamento é indicado pelo acendimento de um LED, sendo ligado e desligado quando apertamos um botão.

Para executá-lo, devemos rodar o comando make e depois rodar o programa em sudo (uma vez que o GPIO só funciona dessa forma)

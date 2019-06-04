# trabalhosSTR
Trabalhos da disciplina de STR

Integrantes
- Felipe Lins
- Luis Henrique
- Matheus Oliveira

Estes trabalhos foram feitos na placa *BeagleBone Black* usando a biblioteca C++ BlackLib-Modificada disponível [neste](https://github.com/ruteee/BlackLib-Modificada) repositório e a biblioteca Python disponível [neste](https://github.com/adafruit/adafruit-beaglebone-io-python) outro repositório.

## Trabalho 1

Criar um programa na *Beaglebone Black* que cumpra os seguintes requisitos:

- Escrita em Pino Digital
- Leitura em Pino Digital
- Uso de Pino PWM
- Uso de Pino ADC
- Log do estados de cada pino

Dessa forma, criamos um programa que lê a tensão sobre um potenciômetro emite o sinal proporcional por meio de um PWM a um LED. Seu funcionamento é indicado pelo acendimento de um LED, sendo ligado e desligado quando apertamos um botão.

Para executá-los, rodamos o make e depois executamos o programa em root (uma vez que o GPIO só funciona dessa forma)

## Trabalho 2

Criar o jogo *Genius* na *BeagleBone* com três leds para indicar a sequência, 3 botões (um relacionado a cada led) para o usuário indicar a sequência, 1 potenciômetro para determinar o tempo de resposta, um botão de reset e um display de 7 segmentos para mostrar a pontuação.

Ao iniciar, o jogador deve informar o tamanho da sequência ara o programa. Após cada sequência aparecer, o jogador tem 3 segundos para se preparar para responder e um tempo de resposta que depende do tamanho da sequência e do valor obtido do potenciômetro.

O vídeo desse trabalho pode ser visto neste [link](https://www.youtube.com/watch?v=N6DKfNhe2gw)

## Trabalho 3

Este trabalho objetiva aplicar o conhecimento de *Threads* e prioridade em um programa feito na *BeagleBone*. Ele consiste na criação de duas *threads* cujas prioridades são definidas a cada segundo por dois potenciômetros. Ambas chamam uma função `carga()` (cujo processamento é indicado por um led para cada *thread*) que serve para ocupar tempo da CPU.

O vídeo desse trabalho pode ser visto neste [link](https://youtu.be/VlvBfMAmoKw)

## Trabalho 4

Esse trabalho consistia em simular quatro trilhos de trens em que alguns de seus segmentos eram compartilhados, conforme podemos ver na imagem a seguir, usando a *BeagleBone*, de modo que não houvesse *Deadlocks* nem trens usando o mesmo segmento de trilho simultaneamente.

![trilhos](/trabalho4/trilhos.png?raw=true)

Cada trem foi representado por uma *thread* e a passagem deles por cada segmento era representada pelo acendimento de um Led. A velocidade deles eram definidas por quatro potenciômetros, cujos valores eram obtidos a cada segundo. 

Para que os trens não usassem o mesmo trilho ao mesmo tempo, cada trem (*thread*) deve reservar o uso do segmento de trilho (recurso) usando *mutex*. Além disso, o trem não libera o trilho em que ele está antes de obter acesso ao próximo trilho. Para evitar *Deadlocks*, o trens 1 e 3 reservam, respectivamente, os trilhos 3 e 5 antes dos trilhos 2 e 8.

Mais informações poderão ser encontradas neste [video](https://youtu.be/jbpJQl0N46s).

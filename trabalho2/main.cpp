#include <iostream>
#include "display.h"
#include <unistd.h>
#include <new>
#include <queue>
#include <cstdlib>
//#include <ctime> 
#include <signal.h>
#include <sys/time.h>
#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include "../../PWM/PWM.h"

using namespace std;
using namespace BlackLib;

enum leds {
	LEFT=0,
	CENTER,
	RIGHT
};

float difftime(struct timeval &fim, struct timeval &comeco)
{
	float diferenca = fim.tv_sec - comeco.tv_sec; 
    diferenca = diferenca + (fim.tv_usec - comeco.tv_usec) * 1e-6; 
    return diferenca;
}

int main()
{
	srand(time(NULL));
	ADC potenciometro(AIN0);
	PWM indicador_tempo(P9_22);
	indicador_tempo.setState(statePwm::stop);

	//GPIO dos botões
	BlackGPIO botoes[4] = {
							BlackGPIO(GPIO_47, input), 
	                        BlackGPIO(GPIO_27, input), 
	                        BlackGPIO(GPIO_7, input), 
	                        BlackGPIO(GPIO_20, input)
	                      };
	
	//GPIO dos leds                        
	BlackGPIO pinos_leds[3] = {
						BlackGPIO(GPIO_30, output),
						BlackGPIO(GPIO_31, output),
						BlackGPIO(GPIO_48, output)
						};
	                        
    //Indica os pinos do display
	gpioName portas_display[8] = {GPIO_22, GPIO_15, GPIO_14, GPIO_4, GPIO_5, GPIO_49, GPIO_3, GPIO_51};    
    Display display(portas_display, 8);  
    
    int pontos = 4;
    int n_sequencia = 6;
    
    //Continuar enquanto o jogador não tiver perdido nem ganhado
    while(pontos < 10 && pontos > 0)
    {
		// Squencia dos leds que irão acender
		int * sequencia_leds = new int[n_sequencia];
		
	  	// Sequencia inserida pelo jogador
	  	queue<int> jogadas;
	  	
        // Váriáveis usadas para contabilizar o tempo
	  	//time_t comeco_do_turno;
	  	//time_t tempo_atual;
	  	struct timeval comeco_do_turno, tempo_atual;
	  
		display.show(pontos);
      
	  	//Espera 2 segundos antes de começar
		sleep(2);
        
        //Tempo total de resposta que varia de 400 ms até 1.5s por elemento da sequencia
        float timeout = (0.5 + potenciometro.getPercentValue()/100)*n_sequencia;
        cout << "O tempo de resposta total será de : " << timeout << " segundos" << endl;
	  	
      	//Escolhe randomicamente sequencia de leds
		for (int i = 0; i < n_sequencia ; i++)
		{
			sequencia_leds[i] = rand()%3;
		}

		//Acende os leds na sequencia escolhida acima
		for (int i = 0; i < n_sequencia ; i++)
		{
		     // Acender o led, dar sleep, apaga led
		     pinos_leds[sequencia_leds[i]].setValue(high);
		     usleep(500000);
		     pinos_leds[sequencia_leds[i]].setValue(low);
		     usleep(500000);
		}
			  
	  	//Espera 3 s entre sequencia e repetição (conforme requisitado)
	  	sleep(3);
		
		//time(&comeco_do_turno);
		//time(&tempo_atual);
		gettimeofday(&comeco_do_turno, NULL);
		gettimeofday(&tempo_atual, NULL);
		
		bool reset = false; 
		pid_t processo = fork();
		
		//Processo filho lida com PWM enquanto o pai com os botões		
		if(processo == 0)
		{
            //Modifica frequencia do indicador do led tempo dependendo do tempo restante
			int periodo = 0;
			indicador_tempo.setState(statePwm::run);
			while(difftime(tempo_atual,comeco_do_turno) < timeout)
			{
				//Multiplica diferença por 2 e trunca para que haja variação de frequência de PWM
				//a cada 0.5 s
				periodo = ((int)((timeout - difftime(tempo_atual, comeco_do_turno))*2))*25000000;
				
				//Estipula periodo mínimo de PWM
				if(periodo >= 50000000)
				{
					indicador_tempo.setPeriod(periodo);    	
					indicador_tempo.setDutyCycle(periodo/2);
				}
				//time(&tempo_atual);
				gettimeofday(&tempo_atual, NULL);
			}			
			
			exit(1);
		}
		else
		{	 
			cout << "Repita a sequência exibida" << endl;
            //Loop enquanto a diferença de tempo for menor que "timeout" e o número de elementos na sequência inserida
            //for inferior a sequencia gerada randomicamente
			while(difftime(tempo_atual,comeco_do_turno) < timeout && jogadas.size() < (unsigned) n_sequencia)
			{		
				//LED 1
				if(botoes[LEFT].isHigh())
				{
                    //Aguarda o botão ser solto e verifica se o intervalo ainda é menor que o tempo total,
                    //antes de inserir elemento na fila
					while(botoes[LEFT].isHigh());
					//time(&tempo_atual);
					gettimeofday(&tempo_atual, NULL);
					if(difftime(tempo_atual, comeco_do_turno) > timeout) break;
					cout << "Botão 0 pressionado" << endl;
					jogadas.push(LEFT);
				}
				//LED 2
			  	else if (botoes[CENTER].isHigh())
				{
                    //Aguarda o botão ser solto e verifica se o intervalo ainda é menor que o tempo total,
                    //antes de inserir elemento na fila
					while(botoes[CENTER].isHigh());
					//time(&tempo_atual);
					gettimeofday(&tempo_atual, NULL);
					if(difftime(tempo_atual, comeco_do_turno) > timeout) break;
					cout << "Botão 1 pressionado" << endl;
					jogadas.push(CENTER);
				}
				//LED 3
			  	else if (botoes[RIGHT].isHigh())
				{
                  	//Aguarda o botão ser solto e verifica se o intervalo ainda é menor que o tempo total,
                    //antes de inserir elemento na fila
					while(botoes[RIGHT].isHigh());
					//time(&tempo_atual);
					gettimeofday(&tempo_atual, NULL);
					if(difftime(tempo_atual, comeco_do_turno) > timeout) break;
					cout << "Botão 2 pressionado" << endl;
					jogadas.push(RIGHT);
				}
				//RESET
				else if (botoes[3].isHigh())
				{
                    //Aguarda botão ser solto, em seguida mudando a flag "reset" para true e parando o loop
					while(botoes[3].isHigh());
					cout << "Reset pressionado" << endl;
					reset = true;
					break;
				}
				//time(&tempo_atual);
				gettimeofday(&tempo_atual, NULL);
			}
          
            //Finaliza processo filho manualmente se o tempo de inserção tiver sido menor que o tempo total
			kill(processo, SIGKILL);
		}
		
        //Desliga PWM
		indicador_tempo.setState(statePwm::stop);	
      
      	//Pára programa se o botão reset for pressionado
		if(reset) break;
		
        //Esvazia fila da sequencia inserida pelo jogador conferindo quantos foram inseridos
        //corretamente
		int acertos = 0;
		int i = 0;
		while(!jogadas.empty())
		{
			if(sequencia_leds[i] == jogadas.front())
			{
		    	acertos++;
			}
			jogadas.pop();
			i++;
		}
	  
        //Incrementa ou Decrementa os pontos se o jogador tiver acertado toda a sequencia
	  	if(acertos == n_sequencia)
	  	{
			pontos++;
			cout << "Você acertou" << endl;
		}
	  	else
	  	{
			pontos--;
			cout << "Você errou" << endl;
		}
	  
		
		//deleta sequência de leds
		delete[] sequencia_leds;
    }
    
    //Pára PWM no fim do jogo;
	indicador_tempo.setState(statePwm::stop);

    //Verifica se o jogador ganhou ou perdeu
	if(pontos == 0)
    {
		cout << "Você perdeu" << endl;
    	//Enquanto reset não for pressionado, pisca o valor 0 no display
    	while(!botoes[3].isHigh())
    	{
			display.show(0);
			
			if(botoes[3].isHigh()) break;
			
			usleep(500000);
			
			display.clear();
			
			if(botoes[3].isHigh()) break;
			
			usleep(500000);
		}
    }
  	else if(pontos == 10)
    {
		cout << "Você ganhou" << endl;
    	//Enquanto reset não for pressionado, pisca leds e o valor 9 no display
    	while(!botoes[3].isHigh())
    	{
			display.show(9);
			pinos_leds[LEFT].setValue(high);		
			pinos_leds[CENTER].setValue(high);		
			pinos_leds[RIGHT].setValue(high);
			
			if(botoes[3].isHigh()) break;
			
			usleep(500000);
			
			display.clear();
			pinos_leds[LEFT].setValue(low);		
			pinos_leds[CENTER].setValue(low);		
			pinos_leds[RIGHT].setValue(low);
			
			if(botoes[3].isHigh()) break;
			
			usleep(500000);
		}
    }
    

	//Limpa display e apaga leds
    display.clear();
    pinos_leds[LEFT].setValue(low);		
	pinos_leds[CENTER].setValue(low);		
	pinos_leds[RIGHT].setValue(low);
    cout << "fim" << endl;
    return 0;
}




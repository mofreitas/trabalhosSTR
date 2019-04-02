#include <iostream>
#include "display.h"
#include <unistd.h>
#include <new>
#include <queue>
#include <cstdlib>
#include <ctime> 
#include <signal.h>
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

//ganhando: piscar tudo	
void ganhador(Display &disp, BlackGPIO *pinos_leds, BlackGPIO)
{
	
	while(true)
	{
		disp.show(9);
		pinos_leds[LEFT].setValue(high);		
		pinos_leds[CENTER].setValue(high);		
		pinos_leds[RIGHT].setValue(high);
		
		usleep(500000);
		
		disp.clear();
		pinos_leds[LEFT].setValue(low);		
		pinos_leds[CENTER].setValue(low);		
		pinos_leds[RIGHT].setValue(low);
		
		usleep(500000);
	}
	
}

void perdedor(Display &disp, const BlackGPIO &reset)
{
	disp.show(0);
		
	usleep(500000);
	
	disp.clear();
	
	usleep(500000);
}	  

int main()
{
	srand(time(NULL));
	ADC potenciometro(AIN0);
	PWM indicador_tempo(P9_22);
	indicador_tempo.setState(statePwm::stop);

	BlackGPIO botoes[4] = {
							BlackGPIO(GPIO_47, input), 
	                        BlackGPIO(GPIO_27, input), 
	                        BlackGPIO(GPIO_7, input), 
	                        BlackGPIO(GPIO_20, input)
	                      };
	                        
	BlackGPIO pinos_leds[3] = {
						BlackGPIO(GPIO_30, output),
						BlackGPIO(GPIO_31, output),
						BlackGPIO(GPIO_48, output)
						};
	                        
    //Insere pinos de 1-8 do display
	gpioName portas_display[8] = {GPIO_22, GPIO_15, GPIO_14, GPIO_4, GPIO_5, GPIO_49, GPIO_3, GPIO_51};    
    Display display(portas_display, 8);
    
	float dificuldade = potenciometro.getPercentValue();
    
    
    int fase = 4;
    int n_sequencia = 2;
    
    while(fase < 10 && fase > 0)
    {
		//Deve ser preenchido aleatóriamente
		int * sequencia_leds;
	  	
	  	// Nível (Fase) que o jogador se encontra
	  	// Ele começa na fase 5
	  	
	  
	  	// Sequencia do jogador
	  	queue<int> jogadas;
	  	
	  	time_t comeco_do_turno;
	  	time_t tempo_atual;
	  
		// 1. Turno do computador
	  
		// 1.1 Gerar a sequencia aleatória de leds
		
		display.show(fase);
	  	sequencia_leds = new int[n_sequencia];
	  	
	  	//espera antes de começar
		sleep(2);
	  	
		for (int i = 0; i < n_sequencia ; i++)
		{
			sequencia_leds[i] = rand()%3;
		}

		// 1.2 Ligar os leds na sequencia

		for (int i = 0; i < n_sequencia ; i++)
		{
		     // Acender o led, dar sleep, apaga led
		     pinos_leds[sequencia_leds[i]].setValue(high);
		     usleep(500000);
		     pinos_leds[sequencia_leds[i]].setValue(low);
		     usleep(500000);
		}

		// 1.3 Apagar os leds

		// 1.4 Bota o contador para contar
		// Ligar o contador 


		// 2.  Turno do humano
		  
		
		
		//A dificuldade é um número entre 0 e 100. O tempo por elemento
		//na sequencia varia de 500 ms até 1.5s.
		//int sequencia = leds_acesos.size();
		int timeout = (0.5 + 1*dificuldade/100)*n_sequencia;
		//mutex mtx;
		//bool timeout = false;
		
		//thread th1(buttonHandler, &mtx, botoes, &timeout, jogadas); 
		//th1.join();  
	  
	  	// 2.  Turno do humano
	  	//espera 3 s entre sequencia e repetição (conforme requisitos)
	  	sleep(3);
		cout << "Repita a sequência" << endl;
		
		time(&comeco_do_turno);
		time(&tempo_atual);
		
		int periodo = 0;
		pid_t processo = fork();
		
		//processo filho lida com PWM
		
		if(processo == 0)
		{
			int periodo = 0;
		indicador_tempo.setState(statePwm::run);
			while(difftime(tempo_atual,comeco_do_turno) < timeout)
			{
				periodo = (timeout - difftime(tempo_atual, comeco_do_turno))*50000000;
				indicador_tempo.setPeriod(periodo);    	
				indicador_tempo.setDutyCycle(periodo/2);
				time(&tempo_atual);
			}			
			
			exit(1);
		}
		else
		{	  
			bool reset = false;  
			while(difftime(tempo_atual,comeco_do_turno) < timeout && jogadas.size() < n_sequencia)
			{		
				//LED 1
				if(botoes[LEFT].isHigh())
				{
					while(botoes[LEFT].isHigh());
					time(&tempo_atual);
					if(difftime(tempo_atual, comeco_do_turno) > timeout) break;
					cout << "Botão 0 pressionado" << endl;
					jogadas.push(LEFT);
				}
				//LED 2
			  	else if (botoes[CENTER].isHigh())
				{
					while(botoes[CENTER].isHigh());
					time(&tempo_atual);
					if(difftime(tempo_atual, comeco_do_turno) > timeout) break;
					cout << "Botão 1 pressionado" << endl;
					jogadas.push(CENTER);
				}
				//LED 3
			  	else if (botoes[RIGHT].isHigh())
				{
					while(botoes[RIGHT].isHigh());
					time(&tempo_atual);
					if(difftime(tempo_atual, comeco_do_turno) > timeout) break;
					cout << "Botão 2 pressionado" << endl;
					jogadas.push(RIGHT);
				}
				//RESET
				else if (botoes[3].isHigh())
				{
					while(botoes[3].isHigh());
					cout << "Reset pressionado" << endl;
					reset = true;
					break;
				}
				time(&tempo_atual);
			}
			kill(processo, SIGKILL);	    
			if(reset) break;
		}
		
		indicador_tempo.setDutyCycle(0);
		
		//ganhando: piscar tudo
		//perdendo: pisca zero
		//começar: aparece ponto, termino: some ponto
		
		// 2.1 Receber entradas

		// 2.2 Comparar com a do computador

		// 2.3. Ver se o cara acertou ou higou com o jogo e aumentar/diminuir o score
			// Se ele acertou fase++
	  		// se ele errou, fase--
		int acertos = 0;
		int i = 0;
		while(!jogadas.empty())
		{
		  	cout << "jogada: " <<  jogadas.front() << endl;
			if(sequencia_leds[i] == jogadas.front())
			{
		    	acertos++;
			}
			jogadas.pop();
			i++;
		}
	  
	  	if(acertos == n_sequencia)
		  fase++;
	  	else
		  fase--;
	  
		
		// 2.4 resetar a sequencia de leds
		delete[] sequencia_leds;
		
    }
    
    
	indicador_tempo.setState(statePwm::stop);
    // 3.   Ver o estado do jogo 

    // 3.1  Ver se ele perdeu/ganhou
	if(fase == 0)
    {
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
  	else if(fase == 10)
    {
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
    // 3.2  Zera o timer e para ele


    display.clear();
    cout << "fim" << endl;
    return 0;
}




#include <iostream>
#include "display.h"
#include <unistd.h>
#include <thread>
#include <mutex>
#include <new>
#include <queue>
#include <cstdlib>
#include <ctime> 
//#include "ButtonHandler.cpp"
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

/*void buttonHandler(mutex *mtx, BlackGPIO *botoes, bool *timeout)
{
	while(true)
    {
    	mtx->lock();
			if(*timeout) break;
    	mtx->unlock();
    	//LED 1
    	if(botoes[LEFT].isHigh())
        {
        	while(botoes[LEFT].isHigh());
        	mtx->lock();
				if(*timeout) break;
    		mtx->unlock();
        	cout << "Botão 0 pressionado" << endl;
        	jogadas.push(LEFT);
        }
    	//LED 2
      	else if (botoes[1].isHigh())
        {
        	while(botoes[1].isHigh());
        	mtx->lock();
				if(*timeout) break;
    		mtx->unlock();
        	cout << "Botão 1 pressionado" << endl;
        	jogadas.push(CENTER);
        }
    	//LED 3
      	else if (botoes[2].isHigh())
        {
        	while(botoes[2].isHigh());
        	mtx->lock();
				if(*timeout) break;
    		mtx->unlock();
        	cout << "Botão 2 pressionado" << endl;
        	jogadas.push(RIGHT);
        }
    	//RESET
        else if (botoes[3].isHigh())
        {
        	while(botoes[3].isHigh());
        	mtx->lock();
				if(*timeout) break;
    		mtx->unlock();
        	cout << "Reset pressionado" << endl;
        	break;
        }
    }
}*/

//ganhando: piscar tudo	
void ganhador(Display &disp, BlackGPIO *pinos_leds)
{
	
	while(true)
	{
		disp.show('9');
		pinos_leds[LEFT].setValue(high);		
		pinos_leds[CENTER].setValue(high);		
		pinos_leds[RIGHT].setValue(high);
		
		usleep(500000);
		
		disp.clear();
		pinos_leds[LEFT].setValue(low);		
		pinos_leds[CENTER].setValue(low);		
		pinos_leds[RIGHT].setValue(low);
	}
	
}

void perdedor(const Display &disp, const BlackGPIO &reset)
{
	
}	  

/*void countTime(const int time_micro, bool *timeout, mutex *mtx)
{
	usleep(time_micro);
	mtx->lock();
		*timeout = true;
	mtx->unlock();
}*/

int main()
{
	srand(time(NULL));
	ADC potenciometro(AIN0);
	PWM indicador_tempo(P9_22);
	indicador_tempo.setState(statePwm::run);

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
    
    //Deve ser preenchido aleatóriamente
	int * sequencia_leds;
  	
  	// Nível (Fase) que o jogador se encontra
  	// Ele começa na fase 5
  	int fase = 5;   	
  	
  
  	// Sequencia do jogador
  	queue<int> jogadas;
  	
  	time_t comeco_do_turno;
  	time_t tempo_atual;
  
    // 1. Turno do computador
  
	// 1.1 Gerar a sequencia aleatória de leds
	
  	sequencia_leds = new int[fase];
  	
	for (int i = 0; i < fase ; i++)
	{
	    sequencia_leds[i] = rand()%3;
	}

    // 1.2 Ligar os leds na sequencia

	for (int i = 0; i < fase ; i++)
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
	//na sequencia varia de 200 ms até 1s.
	//int sequencia = leds_acesos.size();
	int timeout = (0.5 + 1*dificuldade/100)*fase;
	//mutex mtx;
	//bool timeout = false;
	
	//thread th1(buttonHandler, &mtx, botoes, &timeout, jogadas); 
	//th1.join();  
  
  	// 2.  Turno do humano
	cout << "Repita a sequência" << endl;
	
	time(&comeco_do_turno);
	time(&tempo_atual);
	
	cout << difftime(tempo_atual,comeco_do_turno) << ": " << timeout << endl;
	int periodo = 0;
	
	while(difftime(tempo_atual,comeco_do_turno) < timeout && jogadas.size() < fase)
    {
    	periodo = (timeout-difftime(tempo_atual,comeco_do_turno))*100000000;
    	indicador_tempo.setPeriod(periodo);    	
		indicador_tempo.setDutyCycle(periodo/2);
    	
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
        	break;
        }
        time(&tempo_atual);
    }
    
    cout << difftime(tempo_atual,comeco_do_turno) << ": " << timeout << endl;
    indicador_tempo.setDutyCycle(0);
	indicador_tempo.setState(statePwm::stop);
    
    //ganhando: piscar tudo
    //perdendo: pisca zero
    //começar: aparece ponto, termino: some ponto
    
    // 2.1 Receber entradas

    // 2.2 Comparar com a do computador

    // 2.3. Ver se o cara acertou ou higou com o jogo e aumentar/diminuir o score
		// Se ele acertou fase++
  		// se ele errou, fase--
    int acertos = 0;
    cout << "jogadas size: " << jogadas.size() << endl;
    for (int i = 0; i < fase; i++)
    {
      	cout << "jogada: " <<  jogadas.front() << endl;
    	if(sequencia_leds[i] == jogadas.front())
		{
        	acertos++;
    	}
    	jogadas.pop();
    }
  
  	if(acertos == fase)
      fase++;
  	else
      fase--;
  
    
    // 2.4 resetar a sequencia de leds
    delete[] sequencia_leds;
    // 3.   Ver o estado do jogo 

    // 3.1  Ver se ele perdeu/ganhou
	if(fase == 0)
    {
      // PERDEU
    }
  	else if(fase == 10)
    {
      // GANHOU
    }
    // 3.2  Zera o timer e para ele


    
    cout << "chega disso" << endl;
   
    
    cout << "pq não termina" << endl;
    return 0;
}




 /*
    display.show('0');
    sleep(1);
    display.show('1');
    sleep(1);
    display.show('2');
    sleep(1);
    display.show('3');
    sleep(1);
    display.show('4');
    sleep(1);
    display.show('5');
    sleep(1);
    display.show('6');
    sleep(1);
    display.show('7');
    sleep(1);
    display.show('8');
    sleep(1);
    display.show('9');
    sleep(1);
    display.ponto();
    sleep(1);
    display.ponto();
    
    display.clear();   
    
    */


#include <iostream>
#include "display.h"
#include <unistd.h>
#include <thread>
#include <mutex>
//#include "ButtonHandler.cpp"
#include "../../BlackGPIO/BlackGPIO.h"

using namespace std;
using namespace BlackLib;

enum leds {
   			LEFT=0,
  			CENTER,
  			RIGHT
		  };

void countTime(const int time_micro, bool *timeout, mutex *mtx)
{
	sleep(time_micro);
  	mtx->lock();
  		*timeout = true;
  	mtx->unlock();
}

int main()
{
	BlackGPIO botoes[4] = {BlackGPIO(GPIO_22, input), 
	                        BlackGPIO(GPIO_27, input), 
	                        BlackGPIO(GPIO_7, input), 
	                        BlackGPIO(GPIO_20, input)};
	                        
    //Insere pinos de 1-8
    gpioName portas_display[8] = {GPIO_2, GPIO_15, GPIO_14, GPIO_4, GPIO_5, GPIO_49, GPIO_3, GPIO_51};
    
    int razao = 2;
    bool termino = false;
    
    Display display(portas_display, 8);
    
    bool leds_acesos[3] = {
      						false, false, false
    					  };   
  
  	
  
  	
  	
    // 1. Turno do computador
  
    // 1.1 Gerar a sequencia aleatória de leds

    for (int i = LEFT, int i <= RIGHT; i++)
    {
        leds_acesos[i] = (rand()%2 == 0 )? true : false;
    }

    // 1.2 Ligar os leds na sequencia

    for (int i = LEFT, int i <= RIGHT; i++)
    {
         // Acender o led

         // Dar um sleep
           
         //apagar o led
    }

    // 1.3 Apagar os leds

    // 1.4 Bota o contador para contar
    // Ligar o contador 


    // 2.  Turno do humano
      
    
    bool timeout = false;
	int time = 10;
    mutex mtx;
  	thread th(countTime, time, &timeout, &mtx);
  
  	th.detach();
  
    cout << "aperte os butões" << endl;
  	mtx.lock();
  	while(!timeout)
    {
    	mtx.unlock();
    	if(botoes[0].isHigh())
        {
        	while(botoes[0].isHigh());
        }
      	else if (botoes[1].isHigh())
        {
        	while(botoes[1].isHigh());
        }
      	else if (botoes[2].isHigh())
        {
        	while(botoes[2].isHigh());
        }
        else if (botoes[3].isHigh())
        {
        	while(botoes[3].isHigh());
        }
    }
    
    cout << "afim disso" << endl;
   
    
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


#include <new>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include "../../BlackGPIO/BlackGPIO.h"

using namespace std;
using namespace BlackLib;

class ButtonHandler
{
private:
	BlackGPIO **pinos;
	int num_pinos;
	
	int index;
	
	int *ordem_teorica;
	int *ordem_pratica;	
	int waitTime;
	int tamanho;
	
	pthread_t *threads;
	mutex mtx_index;
	mutex mtx_termino;
	
	struct threadsArgs
	{
	    BlackGPIO button;
	    int id;
	} args;

	void* buttonHandler(void* args)
	{			
	    struct threadsArgs *my_data;
            my_data = (struct threadsArgs *) args;
		if(my_data->button.isHigh())
		{
			while(my_data->button.isHigh());
			
			mtx_index.lock();
				ordem_pratica[index] = my_data->id;
			mtx_index.unlock();	
		}
	}

public:

	//Ordem (led1 - led3)
	ButtonHandler(gpioName *portas, int tam)
	{
		//Inicia pinos GPIO
		threads = new pthread_t[tam];
		pinos = new BlackGPIO*[tam];
		num_pinos = tam;	
		
		int i = 0;
		for(i = 0; i < tam; i++)
		{				
		    args.button = pinos[i][0];
		    args.id = i;
			int rc = 0;
			rc = pthread_create(&threads[i], NULL, buttonHandler, (void *) args);
      
			if (rc) {
				cout << "Unable to create thread"<< rc << endl;
				exit(-1);
			}	
			
			pinos[i] = new BlackGPIO(portas[i]);
		}
	}
	
	~ButtonHandler()
	{
		int i = 0;
		
		//Delete pinos
		for(i = 0; i < num_pinos; i++)
		{			
			delete pinos[i];
		}
		
		delete[] pinos;
		
		//Delete Threads
		delete[] threads;
		
		//Delete Threads
		delete[] ordem_pratica;
	}
	
	void set(int* ordem_teorica, int tam, int waitTime)
	{		
		this->ordem_teorica = ordem_teorica;
		this->ordem_pratica = new int[tam];
		tamanho = tam;
		
		int i = 0;
		for(i = 0; i < tamanho, i++)
		{
			this->ordem_pratica[i] = 0;
		}
		
		this->waitTime = waitTime;
	}
	
	bool start()
	{
		int i = 0;
		for(i=0; i<num_pinos; i++)
		{
			pthread_join(threads[i]);
		}
		
		sleep(waitTime);
		
		for(i=0; i<num_pinos; i++)
		{
			pthread_cancel(threads[i]);
		}
		
		for(i = 0; i<tamanho; i++)
		{
			if(ordem_pratica[i] != ordem_teorica[i])
			{
				return false;
			}
		}
		
		return true;
	}	
};


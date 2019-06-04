#include <iostream>
#include <pthread.h>
#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include <unistd.h> //sleep
#include <time.h>   //nanosleep
#include <sched.h>  //sched

#define TEMPO_MIN 500
#define TEMPO_MAX 4500
#define SEGUNDO 1000

using namespace std;
using namespace BlackLib;

enum Trilho{
    trilho1 = 0,
    trilho2,
    trilho3,
    trilho4,    
    trilho5,
    trilho6,
    trilho7,
    trilho8,
    trilho9,
    trilho22,
    trilho33,
    trilho55,
    trilho66,
    trilho88
};

enum Trens
{
    trem1 = 0,
    trem2,
    trem3,
    trem4
};

struct timespec tempo[4];

BlackGPIO leds[14] = {BlackGPIO(GPIO_66, output, FastMode),
                      BlackGPIO(GPIO_67, output, FastMode),
                      BlackGPIO(GPIO_69, output, FastMode),
                      BlackGPIO(GPIO_68, output, FastMode),
                      BlackGPIO(GPIO_45, output, FastMode),
                      BlackGPIO(GPIO_44, output, FastMode),
                      BlackGPIO(GPIO_23, output, FastMode),
                      BlackGPIO(GPIO_26, output, FastMode),
                      BlackGPIO(GPIO_47, output, FastMode),
                      BlackGPIO(GPIO_46, output, FastMode),
                      BlackGPIO(GPIO_4, output, FastMode),
                      BlackGPIO(GPIO_5, output, FastMode),
                      BlackGPIO(GPIO_51, output, FastMode),
                      BlackGPIO(GPIO_48, output, FastMode)};                     

//Mutexes dos trilhos
pthread_mutex_t mtx_trilhos[9] = {PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER};

//Mutexes para alterar velocidade dos trens                          
pthread_mutex_t mutex[4] = {PTHREAD_MUTEX_INITIALIZER,
                            PTHREAD_MUTEX_INITIALIZER,
                            PTHREAD_MUTEX_INITIALIZER,
                            PTHREAD_MUTEX_INITIALIZER};                         

void* trem_func1(void* arg)
{
    struct timespec t;
    while(true)
    {
        //O trilho em que o trem está só é desbloqueado quando
        //ele consegue bloquear o trilho em que ele vai.
        pthread_mutex_lock(&mtx_trilhos[trilho1]);            
        leds[trilho3].setValue(low); 
        leds[trilho1].setValue(high);       
        pthread_mutex_unlock(&mtx_trilhos[trilho3]);
            //Mutex que impede leitura e escrita simultânea pela thread
            //principal e pela thread do trem
            pthread_mutex_lock(&mutex[trem1]); 
            t = tempo[trem1];             
            pthread_mutex_unlock(&mutex[trem1]);
             
            cout << "Trem 1 - trilho 1" << endl;          
            nanosleep(&t, NULL);  
        
        //Reserva s3 e depois s2
        pthread_mutex_lock(&mtx_trilhos[trilho3]);
            pthread_mutex_lock(&mtx_trilhos[trilho2]);                        
            leds[trilho1].setValue(low);    
            leds[trilho2].setValue(high);       
            pthread_mutex_unlock(&mtx_trilhos[trilho1]); 
                pthread_mutex_lock(&mutex[trem1]);           
                t = tempo[trem1];            
                pthread_mutex_unlock(&mutex[trem1]);
                
                cout << "Trem 1 - trilho 2" << endl;  
                nanosleep(&t, NULL);                                       
                leds[trilho2].setValue(low); 
                leds[trilho3].setValue(high);      
            pthread_mutex_unlock(&mtx_trilhos[trilho2]);
                         
            pthread_mutex_lock(&mutex[trem1]);                
            t = tempo[trem1];             
            pthread_mutex_unlock(&mutex[trem1]);  
                 
            cout << "Trem 1 - trilho 3" << endl;   
            nanosleep(&t, NULL);
    }
}

void* trem_func2(void* arg)
{
    struct timespec t; 
    while(true)
    {
        //Reserva s4
        //Após conseguir reservar o trilho, o led do trilho anterior é
        //apagado e o próximo é aceso.
        pthread_mutex_lock(&mtx_trilhos[trilho4]);                           
        leds[trilho22].setValue(low);    
        leds[trilho4].setValue(high);       
        pthread_mutex_unlock(&mtx_trilhos[trilho2]);   
            pthread_mutex_lock(&mutex[trem2]);          
            t = tempo[trem2];                
            pthread_mutex_unlock(&mutex[trem2]);
            
            cout << "Trem 2 - trilho 4" << endl;      
            nanosleep(&t, NULL);
        
        //Reserva s5
        pthread_mutex_lock(&mtx_trilhos[trilho5]);                      
        leds[trilho4].setValue(low);        
        leds[trilho5].setValue(high);         
        pthread_mutex_unlock(&mtx_trilhos[trilho4]);   
            pthread_mutex_lock(&mutex[trem2]);     
            t = tempo[trem2];               
            pthread_mutex_unlock(&mutex[trem2]);
            
            cout << "Trem 2 - trilho 5" << endl;        
            nanosleep(&t, NULL);
        
        //Reserva s6
        pthread_mutex_lock(&mtx_trilhos[trilho6]);     
        leds[trilho5].setValue(low);                 
        leds[trilho6].setValue(high);      
        pthread_mutex_unlock(&mtx_trilhos[trilho5]);     
            pthread_mutex_lock(&mutex[trem2]);    
            t = tempo[trem2];              
            pthread_mutex_unlock(&mutex[trem2]);
            
            cout << "Trem 2 - trilho 6" << endl;           
            nanosleep(&t, NULL);
        
        //Reserva s2
        pthread_mutex_lock(&mtx_trilhos[trilho2]);                       
        leds[trilho6].setValue(low); 
        leds[trilho22].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho6]);
            pthread_mutex_lock(&mutex[trem2]);    
            t = tempo[trem2];            
            pthread_mutex_unlock(&mutex[trem2]);
                         
            cout << "Trem 2 - trilho 2" << endl; 
            nanosleep(&t, NULL);
    }
}

void* trem_func3(void* arg)
{
    struct timespec t;
    while(true)
    {
        //Reserva s7
        pthread_mutex_lock(&mtx_trilhos[trilho7]);     
        leds[trilho55].setValue(low);               
        leds[trilho7].setValue(high);          
        pthread_mutex_unlock(&mtx_trilhos[trilho5]); 
            pthread_mutex_lock(&mutex[trem3]);    
            t = tempo[trem3];               
            pthread_mutex_unlock(&mutex[trem3]);
                 
            cout << "Trem 3 - trilho 7" << endl;  
            nanosleep(&t, NULL);
        
        //Reserva s5 e depois s8
        pthread_mutex_lock(&mtx_trilhos[trilho5]);
            pthread_mutex_lock(&mtx_trilhos[trilho8]);       
            leds[trilho7].setValue(low);               
            leds[trilho8].setValue(high);    
            pthread_mutex_unlock(&mtx_trilhos[trilho7]); 
                pthread_mutex_lock(&mutex[trem3]);       
                t = tempo[trem3];             
                pthread_mutex_unlock(&mutex[trem3]);
                   
                cout << "Trem 3 - trilho 8" << endl;
                nanosleep(&t, NULL);     
                leds[trilho8].setValue(low);
                leds[trilho55].setValue(high);  
            pthread_mutex_unlock(&mtx_trilhos[trilho8]);
            
            pthread_mutex_lock(&mutex[trem3]);         
            t = tempo[trem3];                 
            pthread_mutex_unlock(&mutex[trem3]);  
            
            cout << "Trem 3 - trilho 5" << endl; 
            nanosleep(&t, NULL);
    }
}

void* trem_func4(void* arg)
{
    struct timespec t;
    while(true)
    {
        //Reserva s9
        pthread_mutex_lock(&mtx_trilhos[trilho9]);          
        leds[trilho88].setValue(low);               
        leds[trilho9].setValue(high);    
        pthread_mutex_unlock(&mtx_trilhos[trilho8]); 
            pthread_mutex_lock(&mutex[trem4]);      
            t = tempo[trem4];             
            pthread_mutex_unlock(&mutex[trem4]);
                           
            cout << "Trem 4 - trilho 9" << endl;
            nanosleep(&t, NULL);
        
        //Reserva s3
        pthread_mutex_lock(&mtx_trilhos[trilho3]);       
        leds[trilho9].setValue(low);                
        leds[trilho33].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho9]);
            pthread_mutex_lock(&mutex[trem4]);       
            t = tempo[trem4];             
            pthread_mutex_unlock(&mutex[trem4]);
                           
            cout << "Trem 4 - trilho 3" << endl; 
            nanosleep(&t, NULL);
        
        //Reserva s6
        pthread_mutex_lock(&mtx_trilhos[trilho6]);      
        leds[trilho33].setValue(low);               
        leds[trilho66].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho3]);
            pthread_mutex_lock(&mutex[trem4]);     
            t = tempo[trem4];              
            pthread_mutex_unlock(&mutex[trem4]);
                           
            cout << "Trem 4 - trilho 6" << endl;
            nanosleep(&t, NULL);
        
        //Reserva s8
        pthread_mutex_lock(&mtx_trilhos[trilho8]);                
        leds[trilho66].setValue(low);             
        leds[trilho88].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho6]);  
            pthread_mutex_lock(&mutex[trem4]);       
            t = tempo[trem4];               
            pthread_mutex_unlock(&mutex[trem4]);
                           
            cout << "Trem 4 - trilho 8" << endl;  
            nanosleep(&t, NULL);
    }
}

int main()
{
    pthread_t threads[4];
    ADC potenc[4] = {ADC(AIN0), ADC(AIN1), ADC(AIN2), ADC(AIN3)};
    struct sched_param prioridade;
    prioridade.sched_priority = 5;
    
    //Altera prioridade para 1 usando escalonador de tempo real
    //(Nesse caso, quanto maior o valor, maior a prioridade)
    sched_setscheduler(0 , SCHED_FIFO, &prioridade);
    
    //Criação das threads
    pthread_create(&threads[trem1], NULL, trem_func1, NULL);
    pthread_create(&threads[trem2], NULL, trem_func2, NULL);
    pthread_create(&threads[trem3], NULL, trem_func3, NULL);
    pthread_create(&threads[trem4], NULL, trem_func4, NULL);
    
    //Altera prioridade das threads para serem inferior a thread main
    prioridade.sched_priority = 1;
    pthread_setschedparam(threads[trem1], SCHED_FIFO, &prioridade);
    pthread_setschedparam(threads[trem2], SCHED_FIFO, &prioridade);
    pthread_setschedparam(threads[trem3], SCHED_FIFO, &prioridade);
    pthread_setschedparam(threads[trem4], SCHED_FIFO, &prioridade);
    
    long long valor = 0; 
    
    //Normaliza o valor dos potenciômetros de 500ms a 5000ms, inserindo na posição correspondente ao trem
    //no vetor tempo
    while(true)
    {    
        valor = TEMPO_MIN+TEMPO_MAX*potenc[trem1].getPercentValue()/100.0;
        pthread_mutex_lock(&mutex[trem1]);
            tempo[trem1].tv_sec = valor/SEGUNDO;
            tempo[trem1].tv_nsec = (valor%SEGUNDO)*1000000L;
            cout << "Tempo - trem 1: " << tempo[trem1].tv_sec << ":" << tempo[trem1].tv_nsec << endl;
        pthread_mutex_unlock(&mutex[trem1]);
        
        valor = TEMPO_MIN+TEMPO_MAX*potenc[trem2].getPercentValue()/100.0; 
        pthread_mutex_lock(&mutex[trem2]); 
            tempo[trem2].tv_sec = valor/SEGUNDO;
            tempo[trem2].tv_nsec = (valor%SEGUNDO)*1000000L;  
            cout << "Tempo - trem 2: " << tempo[trem2].tv_sec << ":" << tempo[trem2].tv_nsec << endl;    
        pthread_mutex_unlock(&mutex[trem2]);
        
        valor = TEMPO_MIN+TEMPO_MAX*potenc[trem3].getPercentValue()/100.0; 
        pthread_mutex_lock(&mutex[trem3]);
            tempo[trem3].tv_sec = valor/SEGUNDO;
            tempo[trem3].tv_nsec = (valor%SEGUNDO)*1000000L; 
            cout << "Tempo - trem 3: " << tempo[trem3].tv_sec << ":" << tempo[trem3].tv_nsec << endl;  
        pthread_mutex_unlock(&mutex[trem3]);
        
        valor = TEMPO_MIN+TEMPO_MAX*potenc[trem4].getPercentValue()/100.0;  
        pthread_mutex_lock(&mutex[trem4]);
            tempo[trem4].tv_sec = valor/SEGUNDO;
            tempo[trem4].tv_nsec = (valor%SEGUNDO)*1000000L;              
            cout << "Tempo - trem 4: " << tempo[trem4].tv_sec << ":" << tempo[trem4].tv_nsec << endl;    
        pthread_mutex_unlock(&mutex[trem4]);
                            
        
        sleep(1);
    }
}

#include <iostream>
#include <pthread.h>
#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include <unistd.h>
#include <time.h>

#define TEMPO_MAX 5000
#define SEGUNDO 1000

using namespace std;
using namespace BlackLib;

enum Trilho{
    trilho2 = 0,
    trilho3,    
    trilho5,
    trilho6,
    trilho8,
    trilho1,
    trilho4,
    trilho7,
    trilho9
};

enum Trens
{
    trem1 = 0,
    trem2,
    trem3,
    trem4
};


struct timespec tempo[4];

pthread_mutex_t mtx_trilhos[6] = {PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_MUTEX_INITIALIZER};
                            
pthread_mutex_t mutex[4] = {PTHREAD_MUTEX_INITIALIZER,
                            PTHREAD_MUTEX_INITIALIZER,
                            PTHREAD_MUTEX_INITIALIZER,
                            PTHREAD_MUTEX_INITIALIZER};                            

void* trem_func1(void* arg)
{
    struct timespec t;
    while(true)
    {
        pthread_mutex_lock(&mtx_trilhos[trilho1]);
            pthread_mutex_lock(&mutex[trem1]);
            cout << "trem 1: " << tempo[trem1].tv_sec << ":" << tempo[trem1].tv_nsec << endl;
            t = tempo[trem1];
            pthread_mutex_unlock(&mutex[trem1]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho1]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho3]);
            pthread_mutex_lock(&mtx_trilhos[trilho2]);
                pthread_mutex_lock(&mutex[trem1]);
                cout << "trem 1: " << tempo[trem1].tv_sec << ":" << tempo[trem1].tv_nsec << endl;                  
                t = tempo[trem1]; 
                pthread_mutex_unlock(&mutex[trem1]);
                   
                nanosleep(&t, NULL);
            pthread_mutex_unlock(&mtx_trilhos[trilho2]);
            
            
            pthread_mutex_lock(&mutex[trem1]);
            cout << "trem 1: " << tempo[trem1].tv_sec << ":" << tempo[trem1].tv_nsec << endl;             
            t = tempo[trem1];  
            pthread_mutex_unlock(&mutex[trem1]);  
                 
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho3]);
    }
}

void* trem_func2(void* arg)
{
    struct timespec t;
    while(true)
    {
        pthread_mutex_lock(&mtx_trilhos[trilho4]);
            pthread_mutex_lock(&mutex[trem2]);
            cout << "trem 2: " << tempo[trem2].tv_sec << ":" << tempo[trem2].tv_nsec << endl;            
            t = tempo[trem2];
            pthread_mutex_unlock(&mutex[trem2]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho4]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho5]);
            pthread_mutex_lock(&mutex[trem2]);
            cout << "trem 2: " << tempo[trem2].tv_sec << ":" << tempo[trem2].tv_nsec << endl;        
            t = tempo[trem2];
            pthread_mutex_unlock(&mutex[trem2]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho5]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho6]);
            pthread_mutex_lock(&mutex[trem2]);
            cout << "trem 2: " << tempo[trem2].tv_sec << ":" << tempo[trem2].tv_nsec << endl;        
            t = tempo[trem2];
            pthread_mutex_unlock(&mutex[trem2]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho6]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho2]);
            pthread_mutex_lock(&mutex[trem2]);
            cout << "trem 2: " << tempo[trem2].tv_sec << ":" << tempo[trem2].tv_nsec << endl;        
            t = tempo[trem2];
            pthread_mutex_unlock(&mutex[trem2]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho2]);
    }
}

void* trem_func3(void* arg)
{
    struct timespec t;
    while(true)
    {
        pthread_mutex_lock(&mtx_trilhos[trilho7]);
            pthread_mutex_lock(&mutex[trem3]);
            cout << "trem 3: " << tempo[trem3].tv_sec << ":" << tempo[trem3].tv_nsec << endl;        
            t = tempo[trem3];
            pthread_mutex_unlock(&mutex[trem3]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho7]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho5]);
            pthread_mutex_lock(&mtx_trilhos[trilho8]);
                pthread_mutex_lock(&mutex[trem3]);
                cout << "trem 3: " << tempo[trem3].tv_sec << ":" << tempo[trem3].tv_nsec << endl;         
                t = tempo[trem3];  
                pthread_mutex_unlock(&mutex[trem3]);
                   
                nanosleep(&t, NULL);
            pthread_mutex_unlock(&mtx_trilhos[trilho8]);
            
            
            pthread_mutex_lock(&mutex[trem3]);
            cout << "trem 3: " << tempo[trem3].tv_sec << ":" << tempo[trem3].tv_nsec << endl;          
            t = tempo[trem3]; 
            pthread_mutex_unlock(&mutex[trem3]);  
                 
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho5]);
    }
}

void* trem_func4(void* arg)
{
    struct timespec t;
    while(true)
    {
        pthread_mutex_lock(&mtx_trilhos[trilho9]);
            pthread_mutex_lock(&mutex[trem4]);
            cout << "trem 4: " << tempo[trem4].tv_sec << ":" << tempo[trem4].tv_nsec << endl;        
            t = tempo[trem4];
            pthread_mutex_unlock(&mutex[trem4]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho9]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho3]);
            pthread_mutex_lock(&mutex[trem4]);
            cout << "trem 4: " << tempo[trem4].tv_sec << ":" << tempo[trem4].tv_nsec << endl;        
            t = tempo[trem4];
            pthread_mutex_unlock(&mutex[trem4]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho3]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho6]);
            pthread_mutex_lock(&mutex[trem4]);
            cout << "trem 4: " << tempo[trem4].tv_sec << ":" << tempo[trem4].tv_nsec << endl;        
            t = tempo[trem4];
            pthread_mutex_unlock(&mutex[trem4]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho6]);
        
        pthread_mutex_lock(&mtx_trilhos[trilho8]);
            pthread_mutex_lock(&mutex[trem4]);
            cout << "trem 4: " << tempo[trem4].tv_sec << ":" << tempo[trem4].tv_nsec << endl;        
            t = tempo[trem4];
            pthread_mutex_unlock(&mutex[trem4]);
                           
            nanosleep(&t, NULL);
        pthread_mutex_unlock(&mtx_trilhos[trilho8]);
    }
}

int main()
{
    pthread_t threads[4];
    ADC potenc[4] = {ADC(AIN0), ADC(AIN1), ADC(AIN2), ADC(AIN3)};
    
    pthread_create(&threads[trem1], NULL, trem_func1, NULL);
    pthread_create(&threads[trem2], NULL, trem_func2, NULL);
    pthread_create(&threads[trem3], NULL, trem_func3, NULL);
    pthread_create(&threads[trem4], NULL, trem_func4, NULL);
    
    long long valor = 0;    
    
    while(true)
    {    
        valor = TEMPO_MAX*potenc[trem1].getPercentValue()/100.0;
        pthread_mutex_lock(&mutex[trem1]);
            tempo[trem1].tv_sec = valor/SEGUNDO;
            tempo[trem1].tv_nsec = (valor%SEGUNDO)*1000000L;
        pthread_mutex_unlock(&mutex[trem1]);
        
        valor = TEMPO_MAX*potenc[trem2].getPercentValue()/100.0; 
        pthread_mutex_lock(&mutex[trem2]); 
            tempo[trem2].tv_sec = valor/SEGUNDO;
            tempo[trem2].tv_nsec = (valor%SEGUNDO)*1000000L;      
        pthread_mutex_unlock(&mutex[trem2]);
        
        valor = TEMPO_MAX*potenc[trem3].getPercentValue()/100.0; 
        pthread_mutex_lock(&mutex[trem3]);
            tempo[trem3].tv_sec = valor/SEGUNDO;
            tempo[trem3].tv_nsec = (valor%SEGUNDO)*1000000L;   
        pthread_mutex_unlock(&mutex[trem3]);
        
        valor = TEMPO_MAX*potenc[trem4].getPercentValue()/100.0;  
        pthread_mutex_lock(&mutex[trem4]);
            tempo[trem4].tv_sec = valor/SEGUNDO;
            tempo[trem4].tv_nsec = (valor%SEGUNDO)*1000000L;  
        pthread_mutex_unlock(&mutex[trem4]);
        
        sleep(1);
    }
}

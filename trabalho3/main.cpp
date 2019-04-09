#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include "../../PWM/PWM.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/resource.h>

#define PERIODO 500000000U

using namespace std;
using namespace BlackLib;

int prioridade[2] = {0, 0};
pthread_mutex_t mutex[2] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

struct Parametros
{
    BlackGPIO led = BlackGPIO(GPIO_60, output);
    int tarefaID;
    
    Parametros(int t, BlackGPIO l)
    {
        led = l;
        tarefaID = t;
    }
};

void carga(int N = 3000)
{
    int i, j;
    double soma = 0.0;
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            soma += (double) 0.9*0.9*(1.0/3) - 0.27;
        }
    }
}

void* tarefa(void* params)
{
    struct Parametros *parametro = (struct Parametros*)params;
    while(1)
    {
        pthread_mutex_lock(&mutex[parametro->tarefaID]);
        setpriority(PRIO_PROCESS, 0, prioridade[parametro->tarefaID]);        
        pthread_mutex_unlock(&mutex[parametro->tarefaID]);
        
        cout << "Prioridade Tarefa " << parametro->tarefaID << ": " << getpriority(PRIO_PROCESS, 0) << endl;
        
        parametro->led.setValue(high);
        carga(3000);
        parametro->led.setValue(low);
    }
}

int main()
{    
    pthread_t thread0, thread1;
    ADC potenciometro0 = ADC(AIN0);
    ADC potenciometro1 = ADC(AIN1);
    
    struct Parametros params_thread0(0, BlackGPIO(GPIO_48, output));
    struct Parametros params_thread1(1, BlackGPIO(GPIO_51, output));    
    
    cout << "Iniciando Threads" << endl;
    pthread_create(&thread0, NULL, tarefa, &params_thread0);
    pthread_create(&thread1, NULL, tarefa, &params_thread1);
    
    while(1)
    {
        pthread_mutex_lock(&mutex[0]);
        prioridade[0] = 19*potenciometro0.getPercentValue()/100.0;
        pthread_mutex_unlock(&mutex[0]);
        
        pthread_mutex_lock(&mutex[1]);
        prioridade[1] = 19*potenciometro1.getPercentValue()/100.0;
        pthread_mutex_unlock(&mutex[1]);
        sleep(1);
    }
}

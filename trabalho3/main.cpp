#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include "../../PWM/PWM.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/resource.h>

#define PERIODO 500000000U

using namespace std;
using namespace BlackLib;

//Inicializa vetor que armazena as prioridades de cada thread e um mutex para cada posição
//desse vetor
int prioridade[2] = {0, 0};
pthread_mutex_t mutex[2] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

//Struct para armazenar parâmetros das threads - GPIO do LED e o identificador da thread
struct Parametros
{
    BlackGPIO *led;
    int tarefaID;
    
    Parametros(int t, BlackGPIO *l)
    {
        led = l;
        tarefaID = t;
    }
};

//Função carga com o objetivo de ocupar tempo de CPU
void carga(int N = 4000)
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
    //Converte o parâmetro de ponteiro para void para ponteiro para struct "Parâmetro"
    struct Parametros *parametro = (struct Parametros*)params;
    while(1)
    {
        //Bloqueia o acesso da variável prioridade[parametro->tarefaID] por outras threads
        //enquanto a acessa para obter a prioridade dessa thread.
        pthread_mutex_lock(&mutex[parametro->tarefaID]);
        setpriority(PRIO_PROCESS, 0, prioridade[parametro->tarefaID]);        
        pthread_mutex_unlock(&mutex[parametro->tarefaID]);
        
        cout << "Prioridade Tarefa " << parametro->tarefaID << ": " << getpriority(PRIO_PROCESS, 0) << endl;
        
        parametro->led->setValue(high);
        carga(4000);
        parametro->led->setValue(low);
        carga(4000);
    }
}

int main()
{    
    //Declara threads, ADCs, LEDs e o structs com os parâmetros das threads
    pthread_t thread0, thread1;
    ADC potenciometro0 = ADC(AIN0);
    ADC potenciometro1 = ADC(AIN1);
    BlackGPIO led0 = BlackGPIO(GPIO_48, output);
    BlackGPIO led1 = BlackGPIO(GPIO_51, output);    
    struct Parametros params_thread0(0, &led0);
    struct Parametros params_thread1(1, &led1);  
    
    //Inicia as threads
    cout << "Iniciando Threads" << endl;
    pthread_create(&thread0, NULL, tarefa, &params_thread0);
    pthread_create(&thread1, NULL, tarefa, &params_thread1);
    
    while(1)
    {
        //Bloqueia o acesso ao recurso prioridade[0] pela thread 0 enquanto insere o valor obtido pelo
        //potenciômetro, que é normalizado de 0 a 19 (níveis de prioridade disponíveis). 
        pthread_mutex_lock(&mutex[0]);
        prioridade[0] = 19*potenciometro0.getPercentValue()/100.0;
        pthread_mutex_unlock(&mutex[0]);
        
        //Bloqueia o acesso ao recurso prioridade[1] pela thread 1 enquanto insere o valor obtido pelo
        //potenciômetro, que é normalizado de 0 a 19 (níveis de prioridade disponíveis).  
        pthread_mutex_lock(&mutex[1]);
        prioridade[1] = 19*potenciometro1.getPercentValue()/100.0;
        pthread_mutex_unlock(&mutex[1]);
        
        //Dorme por 1 segundo para obter novos valores de prioridade
        sleep(1);
    }
}

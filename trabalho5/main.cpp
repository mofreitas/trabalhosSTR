#include <iostream>
#include <pthread.h>
#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include <unistd.h> //sleep
#include <time.h>   //nanosleep
#include <sched.h>  //sched
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define TEMPO_MIN 500
#define TEMPO_MAX 4500
#define SEGUNDO 1000
#define MULTICAST_ADDR "225.0.0.225"
#define PORTA_CLIENTE 10000u
#define PORTA_SERVIDOR 10001u

using namespace std;
using namespace BlackLib;

enum Trilho{
    trilho1 = 0,    //trem 1
    trilho2,        //trem 1
    trilho3,        //trem 1
    trilho4,        //trem 2
    trilho5,        //trem 2
    trilho6,        //trem 2
    trilho7,        //trem 3
    trilho8,        //trem 3
    trilho9,        //trem 4
    trilho22,       //trem 2
    trilho33,       //trem 4
    trilho55,       //trem 3
    trilho66,       //trem 4
    trilho88        //trem 4
};

enum Trens
{
    trem1 = 0,
    trem2,
    trem3,
    trem4
};

//vetor com tempo de espera de cada trem nos trilhos (velocidade)
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
             
            //cout << "Trem 1 - trilho 1" << endl;          
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
                
                //cout << "Trem 1 - trilho 2" << endl;  
                nanosleep(&t, NULL);                                       
                leds[trilho2].setValue(low); 
                leds[trilho3].setValue(high);      
            pthread_mutex_unlock(&mtx_trilhos[trilho2]);
                         
            pthread_mutex_lock(&mutex[trem1]);                
            t = tempo[trem1];             
            pthread_mutex_unlock(&mutex[trem1]);  
                 
            //cout << "Trem 1 - trilho 3" << endl;   
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
            
            //cout << "Trem 2 - trilho 4" << endl;      
            nanosleep(&t, NULL);
        
        //Reserva s5
        pthread_mutex_lock(&mtx_trilhos[trilho5]);                      
        leds[trilho4].setValue(low);        
        leds[trilho5].setValue(high);         
        pthread_mutex_unlock(&mtx_trilhos[trilho4]);   
            pthread_mutex_lock(&mutex[trem2]);     
            t = tempo[trem2];               
            pthread_mutex_unlock(&mutex[trem2]);
            
            //cout << "Trem 2 - trilho 5" << endl;        
            nanosleep(&t, NULL);
        
        //Reserva s6
        pthread_mutex_lock(&mtx_trilhos[trilho6]);     
        leds[trilho5].setValue(low);                 
        leds[trilho6].setValue(high);      
        pthread_mutex_unlock(&mtx_trilhos[trilho5]);     
            pthread_mutex_lock(&mutex[trem2]);    
            t = tempo[trem2];              
            pthread_mutex_unlock(&mutex[trem2]);
            
            //cout << "Trem 2 - trilho 6" << endl;           
            nanosleep(&t, NULL);
        
        //Reserva s2
        pthread_mutex_lock(&mtx_trilhos[trilho2]);                       
        leds[trilho6].setValue(low); 
        leds[trilho22].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho6]);
            pthread_mutex_lock(&mutex[trem2]);    
            t = tempo[trem2];            
            pthread_mutex_unlock(&mutex[trem2]);
                         
            //cout << "Trem 2 - trilho 2" << endl; 
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
                 
            //cout << "Trem 3 - trilho 7" << endl;  
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
                   
                //cout << "Trem 3 - trilho 8" << endl;
                nanosleep(&t, NULL);     
                leds[trilho8].setValue(low);
                leds[trilho55].setValue(high);  
            pthread_mutex_unlock(&mtx_trilhos[trilho8]);
            
            pthread_mutex_lock(&mutex[trem3]);         
            t = tempo[trem3];                 
            pthread_mutex_unlock(&mutex[trem3]);  
            
            //cout << "Trem 3 - trilho 5" << endl; 
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
                           
            //cout << "Trem 4 - trilho 9" << endl;
            nanosleep(&t, NULL);
        
        //Reserva s3
        pthread_mutex_lock(&mtx_trilhos[trilho3]);       
        leds[trilho9].setValue(low);                
        leds[trilho33].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho9]);
            pthread_mutex_lock(&mutex[trem4]);       
            t = tempo[trem4];             
            pthread_mutex_unlock(&mutex[trem4]);
                           
            //cout << "Trem 4 - trilho 3" << endl; 
            nanosleep(&t, NULL);
        
        //Reserva s6
        pthread_mutex_lock(&mtx_trilhos[trilho6]);      
        leds[trilho33].setValue(low);               
        leds[trilho66].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho3]);
            pthread_mutex_lock(&mutex[trem4]);     
            t = tempo[trem4];              
            pthread_mutex_unlock(&mutex[trem4]);
                           
            //cout << "Trem 4 - trilho 6" << endl;
            nanosleep(&t, NULL);
        
        //Reserva s8
        pthread_mutex_lock(&mtx_trilhos[trilho8]);                
        leds[trilho66].setValue(low);             
        leds[trilho88].setValue(high);     
        pthread_mutex_unlock(&mtx_trilhos[trilho6]);  
            pthread_mutex_lock(&mutex[trem4]);       
            t = tempo[trem4];               
            pthread_mutex_unlock(&mutex[trem4]);
                           
            //cout << "Trem 4 - trilho 8" << endl;  
            nanosleep(&t, NULL);
    }
}

void* servidor_multicast(void* arg)
{
    int server_sockfd;
    //socklen_t client_len;
    struct sockaddr_in server_address;  
    struct ip_mreq mreq;  // para endereço multicast
    //struct sockaddr_in client_address;  
    
    //Cria um novo socket UDP IPV4
    if((server_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cout << " Houve erro na ebertura do socket " << endl;
        exit(1);
    }
    
    //Define que o servidor será IPV4, receendo conexões de qualquer
    //endereço na porta 10000
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORTA_SERVIDOR);
    
    //Atribui endereço e porta ao socket
    if(bind(server_sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        cout << "Houve error no Bind" << endl;
        exit(1);
    }
    
    //Define endereço multicast e usa setsockopt() para requerer inscrição num grupo multicast com
    //endereço definido em MULTICAST_ADDR recebendo 
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if(setsockopt(server_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) 
    {
        cout << "setsockopt" << endl;
        exit(1);
    }
    cout << " IPPROTO_IP = " << IPPROTO_IP << endl;
    cout << " SOL_SOCKET = " << SOL_SOCKET << endl;
    cout << " IP_ADD_MEMBERSHIP = " << IP_ADD_MEMBERSHIP << endl;
    
    
    while(1){
        float valor_recebido[4];
        
        //client_len = sizeof(client_address);
        //Recebe dados usando socket "server_sockfd" e armazenando em "valor_recebido"
        if(recvfrom(server_sockfd, &valor_recebido, sizeof(valor_recebido),0, /*(struct sockaddr *) &client_address*/ NULL, /*&client_len*/0) < 0)
        {
            cout << " erro no RECVFROM( )" << endl;
            exit(1);
        }
        
        //Escrevendo valores de tempo no vetor de timespec para ser
        //passads para os trens
        int valor = 0;
        valor = TEMPO_MIN+TEMPO_MAX*valor_recebido[trem1];
        pthread_mutex_lock(&mutex[trem1]);
            tempo[trem1].tv_sec = valor/SEGUNDO;
            tempo[trem1].tv_nsec = (valor%SEGUNDO)*1000000L;
            cout << "Trem 1 - " << tempo[trem1].tv_sec << " : " << tempo[trem1].tv_nsec << endl;
        pthread_mutex_unlock(&mutex[trem1]);
        
        valor = TEMPO_MIN+TEMPO_MAX*valor_recebido[trem2];
        pthread_mutex_lock(&mutex[trem2]);
            tempo[trem2].tv_sec = valor/SEGUNDO;
            tempo[trem2].tv_nsec = (valor%SEGUNDO)*1000000L;
            cout << "Trem 2 - " << tempo[trem2].tv_sec << " : " << tempo[trem2].tv_nsec << endl;
        pthread_mutex_unlock(&mutex[trem2]);
        
        valor = TEMPO_MIN+TEMPO_MAX*valor_recebido[trem3];
        pthread_mutex_lock(&mutex[trem3]);
            tempo[trem3].tv_sec = valor/SEGUNDO;
            tempo[trem3].tv_nsec = (valor%SEGUNDO)*1000000L; 
            cout << "Trem 3 - " << tempo[trem3].tv_sec << " : " << tempo[trem3].tv_nsec << endl;
        pthread_mutex_unlock(&mutex[trem3]);
        
        valor = TEMPO_MIN+TEMPO_MAX*valor_recebido[trem4];
        pthread_mutex_lock(&mutex[trem4]);
            tempo[trem4].tv_sec = valor/SEGUNDO;
            tempo[trem4].tv_nsec = (valor%SEGUNDO)*1000000L;
            cout << "Trem 4 - " << tempo[trem4].tv_sec << " : " << tempo[trem4].tv_nsec << endl;
        pthread_mutex_unlock(&mutex[trem4]);
                   
    }
}

int main()
{
    //Variáveis relacionadas ao envio multicast
    int sockfd;
    struct sockaddr_in address;
    
    pthread_t threads[4];
    pthread_t thread_servidor;
    ADC potenc[4] = {ADC(AIN0), ADC(AIN1), ADC(AIN2), ADC(AIN3)};
    struct sched_param prioridade;
    prioridade.sched_priority = 5;
    
    //Altera prioridade para 5 usando escalonador de tempo real
    //(Nesse caso, quanto maior o valor, maior a prioridade)
    //Para a thread de envio e recebimento de dados
    sched_setscheduler(0 , SCHED_FIFO, &prioridade);
    pthread_create(&thread_servidor, NULL, servidor_multicast, NULL);
    pthread_setschedparam(thread_servidor, SCHED_FIFO, &prioridade);
    
    //Criação das threads
    pthread_create(&threads[trem1], NULL, trem_func1, NULL);
    pthread_create(&threads[trem2], NULL, trem_func2, NULL);
    pthread_create(&threads[trem3], NULL, trem_func3, NULL);
    pthread_create(&threads[trem4], NULL, trem_func4, NULL);
    
    //Altera prioridade das threads para serem inferior a thread main
    //(para 1)
    prioridade.sched_priority = 1;
    pthread_setschedparam(threads[trem1], SCHED_FIFO, &prioridade);
    pthread_setschedparam(threads[trem2], SCHED_FIFO, &prioridade);
    pthread_setschedparam(threads[trem3], SCHED_FIFO, &prioridade);
    pthread_setschedparam(threads[trem4], SCHED_FIFO, &prioridade);    
    
    //Criação do socket UDP    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);
    
    //Define endereço e porta de destino dos dados a serem enviados
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    address.sin_port = htons(PORTA_CLIENTE);   
    
    float valores_enviar[4];
    while(true)
    {    
        cout << "Enviando Valores ADC..." << endl;
        valores_enviar[trem1] = potenc[trem1].getPercentValue()/100.0;
        //cout << "trem 1: " << valores_enviar[trem1] << endl;
        
        valores_enviar[trem2] = potenc[trem2].getPercentValue()/100.0;
        //cout << "trem 2: " << valores_enviar[trem2] << endl;

        valores_enviar[trem3] = potenc[trem3].getPercentValue()/100.0;
        //cout << "trem 3: " << valores_enviar[trem3] << endl;
      
        valores_enviar[trem4] = potenc[trem4].getPercentValue()/100.0;
        //cout << "trem 4: " << valores_enviar[trem4] << endl;
        
        //Envia por meio do socket "sockfd" os valores_enviar  no endereço armazenado em address
        sendto(sockfd, &valores_enviar, sizeof(valores_enviar), 0, (struct sockaddr *) &address, sizeof(address));
                           
        sleep(1);
    }
}

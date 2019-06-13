//
//  cliente_multicast.cpp
//  str
//
//  Created by Affonso on 18/05/16.
//  Copyright © 2016 Affonso. All rights reserved.
//

//#include "cliente_multicast.hpp"

// programa cliente_multicast.cpp --> ler um vetor de caracter. Protocolo UDP
//Programa para teste do servidor da placa

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>


#define MULTICAST_ADDR "225.0.0.225"

int main( )
{
    int sockfd;
    int len;
    socklen_t len_recv;
    struct sockaddr_in address;
    
    
    unsigned short porta = 10001;
    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    
    float valor[4] = {0.9, 0.05, 0.3, 0.5};
    
    
    
    while(true)
    {
        sendto(sockfd, &valor,sizeof(valor),0,(struct sockaddr *) &address, len);
        sleep(1);
    }
    close(sockfd);
    exit(0);
}

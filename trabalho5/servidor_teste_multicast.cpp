//
//  servidor_multicast.cpp
//  str
//
//  Created by Affonso on 18/05/16.
//  Copyright © 2016 Affonso. All rights reserved.
//

// #include "servidor_multicast.hpp"

// programa servidor_multicast.cpp - Servidor que lê e escreve em um vetor de caracter
// Protocolo UDP
//Programa teste do cliente da placa

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;


#define MULTICAST_ADDR "225.0.0.225"


int main( )
{
    int server_sockfd;
    size_t server_len;
    socklen_t client_len;
    struct sockaddr_in server_address;
    
    struct ip_mreq mreq;  // para endere�o multicast
    
    unsigned short porta = 10000;
    
    //unlink("server_socket");  // remocao de socket antigo
    if ( (server_sockfd = socket(AF_INET, SOCK_DGRAM, 0) )  < 0  )  // cria um novo socket
    {
        printf(" Houve erro na ebertura do socket ");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(porta);
    
    
    server_len = sizeof(server_address);
    
    if(  bind(server_sockfd, (struct sockaddr *) &server_address, server_len) < 0 )
    {
        cout << "Houve error no Bind" << endl;
        exit(1);
    }
    
    
    // use setsockopt() para requerer inscri��o num grupo multicast
    mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(server_sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
        cout << "setsockopt" <<endl;
        exit(1);
    }
    cout << " IPPROTO_IP = " << IPPROTO_IP << endl;
    cout << " SOL_SOCKET = " << SOL_SOCKET << endl;
    cout << " IP_ADD_MEMBERSHIP = " << IP_ADD_MEMBERSHIP << endl;
    
    
    while(1){
        float valor[4];
        
        cout << "Servidor esperando ...\n" << endl;
        
        if(recvfrom(server_sockfd, &valor, sizeof(valor),0,
                    NULL, 0) < 0 )
        {
            cout << " erro no RECVFROM( )" << endl;
            //exit(1);
        }
        
        //cout << "coisa: " << client_address.sin_addr.s_addr << endl;
        
        for(int i = 0; i<4; i++)
        {
            cout << "valor recebido " << valor[i] << endl;    
        }
        
    }
}

#include <iostream>
#include "display.h"
#include <unistd.h>

using namespace std;
using namespace BlackLib;

int main()
{
    gpioName portas_display[8] = {GPIO_2, GPIO_15, GPIO_14, GPIO_4, GPIO_5, GPIO_49, GPIO_3, GPIO_51};
    Display display(portas_display, 8);
    
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
    
    cout << "pq não termina" << endl;
    return 0;
}

/*

 

*/

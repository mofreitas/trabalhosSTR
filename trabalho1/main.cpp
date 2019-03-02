#include <iostream>
#include <pthread.h>
#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include "../../PWM/PWM.h"
#include <unistd.h>

#define PERIODO 500000

using namespace std;
using namespace BlackLib;

//Espera botão ser pressionado por pelo menos 200ms e ser solto
bool isPressed(BlackGPIO &botao){
    if (botao.isHigh()) {
        cout << "BOTÃO PRESSIONADO" << endl;
        usleep(200000);
        if(botao.isHigh()) { 
            //Aguar botão ser "despressionado para emitir sinal de verdadeiro"     
            while(botao.isHigh()){}
            cout << "BOTÃO SOLTO" << endl;
            return true;
        }
        else{
            cout << "BOTÃO SOLTO" << endl;            
        }
    }
    return false;
}

int main()
{
    // Inicialização dos pinos GPIO, ADC e PWM
    BlackGPIO botao(GPIO_60, input);          
    BlackGPIO led(GPIO_30, output);           
    ADC adc(AIN0);
    PWM pwm(P9_22);
    pwm.setState(statePwm::run);
    float valorADC = 0;
    
    //Bloqueia programa enquanto botão não for pressionado
    while (!isPressed(botao)){}
    cout << "ACENDENDO LED" << endl;
    led.setValue(high);	

    while (!isPressed(botao)){
	    valorADC = adc.getPercentValue();
        cout << "ADC (%): " << valorADC << "; ";
        pwm.setPeriod(PERIODO); //periodo de 500000 ns
        pwm.setDutyCycle(PERIODO*valorADC/100.0);
        cout << "DC PWM (ns): " << pwm.getDutyCycle() << endl;
    }

    cout << "DESLIGANDO LED" << endl;
    led.setValue(low);
    cout << "DESLIGANDO PWM" << endl;
    pwm.setDutyCycle(0);
    pwm.setState(statePwm::stop);
    cout << "FIM" << endl;
    return 0;
}

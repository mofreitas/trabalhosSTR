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
        usleep(200000);
        if(botao.isHigh()) { 
            //Aguar botão ser "despressionado para emitir sinal de verdadeiro"     
            while(botao.isHigh()){}
            return true;
        }
    }
    return false;
}

int main()
{
    BlackGPIO botao(GPIO_60, input);                    // initialization botão
    BlackGPIO led(GPIO_30, output);           // porta 12
    ADC adc(AIN0);
    PWM pwm(P9_22);
    pwm.setState(statePwm::run);
    float valorAdc = 0;
    
    //Bloqueia programa enquanto botão não for pressionado
    while (!isPressed(botao)){}
    led.setValue(high);	

    while (!isPressed(botao)){
	valorAdc = adc.getPercentValue();
        cout << valorAdc << endl;
        pwm.setPeriod(PERIODO); //periodo de 500000 ns
        pwm.setDutyCycle(PERIODO*valorAdc/100.0);
    }

    led.setValue(low);
    pwm.setDutyCycle(0);
    pwm.setState(statePwm::stop);
    return 0;
}

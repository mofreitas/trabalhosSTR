#include <iostream>
#include <pthread.h>
#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include "../../PWM/PWM.h"
#include <unistd.h>

// Inclua as classes que achar necessario

using namespace std;
using namespace BlackLib;

bool isPressed(BlackGPIO &botao){
    if (botao.getValue() == "0")
    {
        usleep(100000);
        if (botao.getValue() == "0")
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{

    BlackGPIO botao(GPIO_71, input, FastMode); // initialization botão
    BlackGPIO led1(GPIO_73, output);           // initialization second output, fast mode
    ADC adc(AIN0);
    PWM pwm(P9_14);

    while (true)
    {
        //Espera botão ser pressionado por pelo menos 100ms
        while (true){
            if(isPressed(botao)){
                break;
            };
        }

        while (!isPressed(botao)){
            led1.setValue(high);
            
            float teste = adc.getPercentValue();
            pwm.setPeriod(1000000); //periodo de 1 ms?
            pwm.setDutyCycle(teste);
            pwm.setState(statePwm::run);
        }
        
        pwm.setState(statePwm::stop);
        led1.setValue(low);

    }
}
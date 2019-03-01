#include <iostream>
#include <pthread.h>
#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include "../../PWM/PWM.h"
#include <unistd.h>
#include <string>

// Inclua as classes que achar necessario

using namespace std;
using namespace BlackLib;

bool isPressed(BlackGPIO &botao){
    if (botao.isHigh())
    {
	cout << "foi1" << endl;
        usleep(1000);
        if (botao.isHigh())
        {
	    cout << "foi2" << endl;
            return true;
        }
    }
    return false;
}

int main()
{

    BlackGPIO botao(GPIO_60, input); // initialization botão
    BlackGPIO led1(GPIO_48, BlackLib::output);           // porta 12
    //ADC adc(AIN0);
    //PWM pwm(P9_14);

    while (true)
    {
        //Espera botão ser pressionado por pelo menos 100ms
        while (true){
            if(isPressed(botao)){
		cout << "brekou" << endl;
                break;
            };
        }

	cout << "sleepou" << endl;
	sleep(1);
	cout << "fim sleep" << endl;
	cout << led1.getDirection() << endl;
	cout << led1.setValue(BlackLib::high) << endl;

        while (!isPressed(botao)){
	    cout << "ligado" << endl;
            
            
            //float teste = adc.getPercentValue();
            //pwm.setPeriod(1000000); //periodo de 1 ms?
            //pwm.setDutyCycle(teste);
            //pwm.setState(statePwm::run);
        }
        
        //pwm.setState(statePwm::stop);
        led1.setValue(BlackLib::low);

    }
}

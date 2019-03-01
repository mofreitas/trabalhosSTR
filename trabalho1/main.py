#coding:utf-8
import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.PWM as PWM
import Adafruit_BBIO.ADC as ADC
import time

#Definindo pinos
PIN_LED = "P9_11"
PIN_BUTTON = "P9_12"
PIN_PWM = "P9_14"
PIN_ADC = "P9_39"

#Configurando os pinos
ADC.setup()                         #Configurando ADC
GPIO.setup(PIN_LED, GPIO.OUT)       #Configurando o pino do LED com saída
GPIO.setup(PIN_BUTTON, GPIO.IN)     #Configurando o pino do botão como entrada
PWM.start(PIN_PWM, 0)               #Configurando PWM com duty cycle = 0

#Funcão que verifica se o botao foi pressiondo por pelo menos 0.5 segundos
def isPressed(pin):
    if GPIO.input(pin):
        time.sleep(0.5)
        if GPIO.input(pin):
            return True
    return False


#O programa inicia e pausa com o aperto do botão, finalizando quando Ctrl+c é pressionado
while True:
    try:  
        #Se botão for pressionado, Led acende e PWM = ADC
        if isPressed(pin=PIN_BUTTON):
            GPIO.output(PIN_LED, GPIO.HIGH)
            
            while(True):                
                value = ADC.read(PIN_ADC)
                PWM.set_duty_cycle(PIN_PWM, value*100.0)
                print(value)

                if(isPressed(pin=PIN_BUTTON)):
                    GPIO.output(PIN_LED, GPIO.LOW) 
                    PWM.set_duty_cycle(PIN_PWM, 0)
                    break 
    except:
        #Define 0 como saída do pino e desativa GPIO
        GPIO.output(PIN_LED, GPIO.LOW)
        GPIO.cleanup()
        #Pára PWM e o desaiva
        PWM.stop(PIN_PWM)
        PWM.cleanup()
        print("fim")
        break

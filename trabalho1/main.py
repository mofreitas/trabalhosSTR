#coding:utf-8
import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.PWM as PWM
import Adafruit_BBIO.ADC as ADC
import time

#Definindo pinos
PIN_LED = "P9_11"
PIN_BUTTON = "P9_12"
PIN_PWM = "P9_22"
PIN_ADC = "P9_39"

#Configurando os pinos
ADC.setup()                         #Configurando ADC
GPIO.setup(PIN_LED, GPIO.OUT)       #Configurando o pino do LED com saída
GPIO.setup(PIN_BUTTON, GPIO.IN)     #Configurando o pino do botão como entrada
PWM.start(PIN_PWM, 0)               #Configurando PWM com duty cycle = 0

#Funcão que verifica se o botao foi pressiondo por pelo menos 0.2 segundos
def isPressed(pin):
    if GPIO.input(pin):
        print("BOTÃO PRESSIONADO")
        time.sleep(0.2)
        if GPIO.input(pin):
            while GPIO.input(pin):
                pass
            print("BOTÃO SOLTO")
            return True
        else:
            print("BOTÃO SOLTO")
    return False


#Se botão for pressionado, Led acende e PWM = ADC*100
while not isPressed(pin=PIN_BUTTON):
    pass
print("ACENDENDO LED")
GPIO.output(PIN_LED, GPIO.HIGH)
           
while not isPressed(pin=PIN_BUTTON):                
    value = ADC.read(PIN_ADC)
    PWM.set_duty_cycle(PIN_PWM, value*100.0)
    print("ADC: " + str(value) + "; PWM (%): " + str(value*100.0)) #Uma vez que não há uma função para obter o valor do PWM
        
#Define 0 como saída do pino e desativa GPIO
print("DESLIGANDO LED")
GPIO.output(PIN_LED, GPIO.LOW)
GPIO.cleanup()
#Pára PWM e o desativa 
#PWM.set_duty_cycle(PIN_PWM, 0) 
print("DESLIGANDO PWM")
PWM.stop(PIN_PWM)
PWM.cleanup()
print("FIM DO PROGRAMA")

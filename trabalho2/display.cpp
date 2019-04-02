#include "display.h"
    
Display::Display(const gpioName *portas, const int tam)
{		
	saidas = new BlackGPIO*[tam];
		
	int i = 0;
	for(i = 0; i < tam; i++)
    {
        saidas[i] = new BlackGPIO(portas[i], output);        
	}
}

Display::~Display(){
    int i = 0;
    for(i = 0; i < 8; i++){
        delete(saidas[i]);
    }    
    delete[](saidas);
}

void Display::show(int digito)
{
	switch(digito)
	{
		case 0:
			saidas[CENTER][0].setValue(low);
			saidas[LEFT_TOP][0].setValue(high);
			saidas[LEFT_BOTTOM][0].setValue(high);
			saidas[BOTTOM][0].setValue(high);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(high);
			break;				
		case 1:	
			saidas[CENTER][0].setValue(low);
			saidas[LEFT_TOP][0].setValue(low);
			saidas[LEFT_BOTTOM][0].setValue(low);
			saidas[BOTTOM][0].setValue(low);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(low);
			break;
		case 2:	
			saidas[CENTER][0].setValue(high);
			saidas[LEFT_TOP][0].setValue(low);
			saidas[LEFT_BOTTOM][0].setValue(high);
			saidas[BOTTOM][0].setValue(high);
			saidas[RIGHT_BOTTOM][0].setValue(low);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(high);
			break;
		case 3:	
			saidas[CENTER][0].setValue(high);
			saidas[LEFT_TOP][0].setValue(low);
			saidas[LEFT_BOTTOM][0].setValue(low);
			saidas[BOTTOM][0].setValue(high);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(high);
			break;
		case 4:	
			saidas[CENTER][0].setValue(high);
			saidas[LEFT_TOP][0].setValue(high);
			saidas[LEFT_BOTTOM][0].setValue(low);
			saidas[BOTTOM][0].setValue(low);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(low);
			break;
		case 5:	
			saidas[CENTER][0].setValue(high);
			saidas[LEFT_TOP][0].setValue(high);
			saidas[LEFT_BOTTOM][0].setValue(low);
			saidas[BOTTOM][0].setValue(high);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(low);
			saidas[TOP][0].setValue(high);
			break;
		case 6:	
			saidas[CENTER][0].setValue(high);
			saidas[LEFT_TOP][0].setValue(high);
			saidas[LEFT_BOTTOM][0].setValue(high);
			saidas[BOTTOM][0].setValue(high);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(low);
			saidas[TOP][0].setValue(high);
			break;
		case 7:	
			saidas[CENTER][0].setValue(low);
			saidas[LEFT_TOP][0].setValue(low);
			saidas[LEFT_BOTTOM][0].setValue(low);
			saidas[BOTTOM][0].setValue(low);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(high);
			break;
		case 8:	
			saidas[CENTER][0].setValue(high);
			saidas[LEFT_TOP][0].setValue(high);
			saidas[LEFT_BOTTOM][0].setValue(high);
			saidas[BOTTOM][0].setValue(high);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(high);
			break;
		case 9:	
			saidas[CENTER][0].setValue(high);
			saidas[LEFT_TOP][0].setValue(high);
			saidas[LEFT_BOTTOM][0].setValue(low);
			saidas[BOTTOM][0].setValue(high);
			saidas[RIGHT_BOTTOM][0].setValue(high);
			saidas[RIGHT_TOP][0].setValue(high);
			saidas[TOP][0].setValue(high);
			break;
		default:
			saidas[CENTER][0].setValue(low);
			saidas[LEFT_TOP][0].setValue(low);
			saidas[LEFT_BOTTOM][0].setValue(low);
			saidas[BOTTOM][0].setValue(low);
			saidas[RIGHT_BOTTOM][0].setValue(low);
			saidas[RIGHT_TOP][0].setValue(low);
			saidas[TOP][0].setValue(low);
			break;	
	}	
}

void Display::ponto()
{
	saidas[PONTO][0].toggleValue();
}
	
void Display::clear()
{
	saidas[CENTER][0].setValue(low);
	saidas[LEFT_TOP][0].setValue(low);
	saidas[LEFT_BOTTOM][0].setValue(low);
	saidas[BOTTOM][0].setValue(low);
	saidas[RIGHT_BOTTOM][0].setValue(low);
	saidas[RIGHT_TOP][0].setValue(low);
	saidas[TOP][0].setValue(low);
	saidas[PONTO][0].setValue(low);
}

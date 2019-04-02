#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "../../BlackGPIO/BlackGPIO.h"

using namespace std;
using namespace BlackLib;

class Display
{
private:
	enum saida
	{
		CENTER = 0,
		LEFT_TOP,
		LEFT_BOTTOM,
		BOTTOM,
		PONTO,
		RIGHT_BOTTOM,
		RIGHT_TOP,
		TOP
	};
		
	//std::vector<BlackGPIO> saidas;
	BlackGPIO **saidas;

public:
	/*
	*	Recebe as entradas relativas aos pinos em ordem:
	*	1, 2, 4, 5, 6, 7, 9, 10 (pinos dos dígitos)
	*/
	Display(const gpioName *portas, const int tam);
	
	~Display();
	
	void show(int digito);
	
	void ponto();
	
	void clear();
};

#endif

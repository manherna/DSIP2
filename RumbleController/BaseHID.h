#pragma once

typedef unsigned short WORD;
class BaseHID
{
public:
	BaseHID();
	~BaseHID();
	
	void Actualiza();




protected:
	bool controllerConnected;
	WORD wButtons;
	WORD wLastButtons;
	WORD wButtonsDown, wButtonsUp;
	//Definición de ejes de joysticks. Los usaremos en el intervalo [-1.0,1.0]
	float fThumbRX, fThumbRY, fThumbLX, fThumbLY;

	//Definición de floats para los triggers. Intervalo [0.0, 1.0]	
	float rTrigger, lTrigger;

	virtual void mandoAHID() = 0;
	virtual void updateController() = 0;
	virtual bool readController() = 0;
	virtual void writeController() = 0;
};


#include "BaseHID.h"
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )
void BaseHID::Actualiza(){
	controllerConnected = readController();
	if (controllerConnected){
		
		mandoAHID();
		//Detectamos gestos

		wButtonsDown = wButtons & ~wLastButtons;
		wButtonsUp = ~wButtons & wLastButtons;

		//aquí hariamos el filtrado de joysticks.
	}

}

BaseHID::BaseHID()
{
}


BaseHID::~BaseHID()
{
}

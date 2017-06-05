#pragma once
#include <Windows.h>
#include "WiiUse\wiiusecpp.h"
#include "BaseHID.h"
#define CONTROLLERS 1

class HIDWii :
	public BaseHID
{
public:
	CWii * base;
	CWiimote * controller;
	bool motorOn;
	HIDWii();
	HIDWii(float t) : BaseHID(t){
		base = new CWii(1);
		base->Find(30);
		
		controller = &base->Connect()[0];
		controller->SetMotionSensingMode(CWiimote::ON);
		controller->Accelerometer.SetGravityCalVector(100, 100, 100);
		controller->SetLEDs(CWiimote::LED_1);
		controller->SetLEDs(CWiimote::LED_2);
		controller->SetLEDs(CWiimote::LED_3);
		controller->SetLEDs(CWiimote::LED_4);
		controller->SetLEDs(CWiimote::LED_1 + CWiimote::LED_2 + CWiimote::LED_3 + CWiimote::LED_4);
	};
	virtual ~HIDWii(){
		delete base;
	};
	bool readController(){
		if (controller != nullptr){
			return true;
		}
		else return false;
	
	}
	void mandoAHID(){
		base->Poll();
		float roll, pitch, yaw;
		controller->Accelerometer.GetOrientation(pitch, roll, yaw);
		fThumbLY = -pitch * 10/ 180;
		fThumbLX = roll * 10 / 180;
		WORD buttons = 0x00000;

		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_A)){
			buttons += WIIMOTE_BUTTON_A;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_B)){
			buttons += WIIMOTE_BUTTON_B;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_PLUS)){
			buttons += WIIMOTE_BUTTON_PLUS;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_MINUS)){
			buttons += WIIMOTE_BUTTON_MINUS;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_HOME)){
			buttons += WIIMOTE_BUTTON_HOME;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_UP)){
			buttons += WIIMOTE_BUTTON_UP;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_DOWN)){
			buttons += WIIMOTE_BUTTON_DOWN;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_RIGHT)){
			buttons += WIIMOTE_BUTTON_RIGHT;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_LEFT)){
			buttons += WIIMOTE_BUTTON_LEFT;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_ONE)){
			buttons += WIIMOTE_BUTTON_ONE;
		}
		if (controller->Buttons.isPressed(WIIMOTE_BUTTON_TWO)){
			buttons += WIIMOTE_BUTTON_TWO;
		}
		wButtons = buttons;
	};
	void writeController(){
		if (lMotor > 0 || rMotor > 0){
			controller->SetRumbleMode(CWiimote::ON);
		}
		else controller->SetRumbleMode(CWiimote::OFF);
	}
};


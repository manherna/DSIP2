#pragma once
#include <Windows.h>
#include "BaseHID.h"
#include "wiiuse_v0.12_win\wiiuse.h"
#define CONTROLLERS 1

class HIDWii :
	public BaseHID
{
public:
	wiimote ** wiimotes;
	BYTE ** buffers;
	bool motorOn;
	HIDWii() :BaseHID(10){ 
		wiimotes = wiiuse_init(CONTROLLERS);
		wiiuse_set_bluetooth_stack(wiimotes, CONTROLLERS, WIIUSE_STACK_MS);
		wiiuse_find(wiimotes, CONTROLLERS, 10);
		wiiuse_connect(wiimotes, CONTROLLERS);
		wiiuse_poll(wiimotes, CONTROLLERS);
	};
	HIDWii(float t) : BaseHID(t){
		wiimotes = wiiuse_init(CONTROLLERS);
		wiiuse_set_bluetooth_stack(wiimotes, CONTROLLERS, WIIUSE_STACK_MS);
		wiiuse_find(wiimotes, CONTROLLERS, 10);
		wiiuse_connect(wiimotes, CONTROLLERS);
		wiiuse_poll(wiimotes, CONTROLLERS);
	};
	virtual ~HIDWii(){};
	bool readController(){
		wiiuse_poll(wiimotes, CONTROLLERS);
		if (wiimotes[0] != nullptr) return true;
		else return false;
	}
	void mandoAHID(){
		for (size_t i = 0; i < CONTROLLERS; i++){
			wButtons = wiimotes[i]->btns;
			wButtonsDown = wiimotes[i]->btns_held;
			wButtonsUp = wiimotes[i]->btns_released;

			fThumbLY = wiimotes[i]->orient.pitch / (MAXINT);
			fThumbLX = wiimotes[i]->accel.x / (MAXBYTE);
		}
	};
	void writeController(){
		if (rMotor > 0 || lMotor > 0){
			wiiuse_rumble(wiimotes[0], 1);
		}
		else wiiuse_rumble(wiimotes[0], 0);
	}
};


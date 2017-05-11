#define STRICT
#include <windows.h>
#include <commdlg.h>
#include <XInput.h>
#include <basetsd.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "resource.h"

#include "HIDXbox.h"
//USAMOS MANDO DE EQUISBOS, ASI QUE DEFINIMOS SUS TECLAS
#define XINPUT_GAMEPAD_DPAD_UP 0x0001 // bit 0
#define XINPUT_GAMEPAD_DPAD_DOWN 0x0002 // bit 1
#define XINPUT_GAMEPAD_DPAD_LEFT 0x0004 // bit 2
#define XINPUT_GAMEPAD_DPAD_RIGHT 0x0008 // bit 3
#define XINPUT_GAMEPAD_START 0x0010 // bit 4
#define XINPUT_GAMEPAD_BACK 0x0020 // bit 5
#define XINPUT_GAMEPAD_LEFT_THUMB 0x0040 // bit 6
#define XINPUT_GAMEPAD_RIGHT_THUMB 0x0080 // bit 7
#define XINPUT_GAMEPAD_LEFT_SHOULDER 0x0100 // bit 8
#define XINPUT_GAMEPAD_RIGHT_SHOULDER 0x0200 // bit 9
#define XINPUT_GAMEPAD_A 0x1000 // bit 12
#define XINPUT_GAMEPAD_B 0x2000 // bit 13
#define XINPUT_GAMEPAD_X 0x4000 // bit 14
#define XINPUT_GAMEPAD_Y 0x8000 // bit 15

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define MAX_CONTROLLERS     4
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )

//Definicion de calbacs
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK updateControllerState();

//Manejador de ventana
HWND g_hWnd;
HIDXbox controller;
WCHAR g_szMessage[4][1024] = { 0 };



int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int)
{

	SetTimer(g_hWnd, 1, 10, (TIMERPROC)updateControllerState);
	// Register the window class
	HBRUSH hBrush = CreateSolidBrush(0xFF0000);
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), 0, MsgProc, 0L, 0L, hInst, NULL,
		LoadCursor(NULL, IDC_ARROW), hBrush,
		NULL, L"XInputSample", NULL
	};
	RegisterClassEx(&wc);

	// Getting the active window
	g_hWnd = GetActiveWindow();

	// Init state
	//ZeroMemory(controllers, sizeof(HIDXbox) * MAX_CONTROLLERS);

	// Enter the message loop
	MSG msg;
	msg.message = WM_NULL;

	while (GetMessage(&msg, NULL, 0U, 0U))
	{
		
			// Translate and dispatch the message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	// Clean up 
	UnregisterClass(L"XInputSample", NULL);

	return 0;
}


//Gestion de mensajes

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATEAPP:
	{
		if (wParam == TRUE)
		{
			// App is now active, so re-enable XInput
			XInputEnable(true);
		}
		else
		{
			// App is now inactive, so disable XInput to prevent
			// user input from effecting application and to 
			// disable rumble. 
			XInputEnable(false);
		}
		break;
	}

	case WM_PAINT:
	{
		// Paint some simple explanation text
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);
		SetBkColor(hDC, 0xFF0000);
		SetTextColor(hDC, 0xFFFFFF);
		RECT rect;
		GetClientRect(hWnd, &rect);

		rect.top = 20;
		rect.left = 20;
		DrawText(hDC,
			L"Use the controller's left/right trigger to adjust the speed of the left/right rumble motor.\n",
			-1, &rect, 0);
		
		//Cambiar este bucle para solo un controlador
		for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
		{
			rect.top = i * 80 + 90;
			rect.left = 20;
			DrawText(hDC, g_szMessage[i], -1, &rect, 0);
		}

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		KillTimer(g_hWnd, 1);
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

VOID CALLBACK updateControllerState(){
		controller.Actualiza();
		if (controller.LT()>0.1)controller.leftMotor(controller.LT());
		else controller.leftMotor(0);
		if (controller.RT()>0.1)controller.rightMotor(controller.RT());
		else controller.rightMotor(0);

		if (controller.BD(XINPUT_GAMEPAD_BACK)){
			PostQuitMessage(0);
		}

		//Movimientos de joystis aqui
		if (controller.LJX != 0){
			
		}
	
		controller.writeController();
}
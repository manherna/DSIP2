//Práctica 2 de Diseño de Sistemas Interactivos
// Gestión de un HID de Xbox one
// Por Manuel Hernández


#include <Windows.h>
#include "HIDXbox.h"
#include "HIDWii.h"
#include <commdlg.h>
#include <basetsd.h>


  //ms para actualizar
#define TARGET_WII
#define T

#ifdef TARGET_XBOX360
#define T 10
HIDXbox controller(T);
#elif defined(TARGET_PS3)
HIDPs controller(T);
#elif defined(TARGET_WII)
#define T 5 
HIDWii controller(T);
#endif

void GeneraEventos(HIDXbox * controller);
void GeneraEventos(HIDWii * controller);


WCHAR g_szMessage[4][1024];
HWND g_hWnd;


VOID CALLBACK  updateControllerState(){
	controller.Actualiza();
	GeneraEventos(&controller);
}
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nCmdShow)
{


	//Me daba problemas al convertir a (TIMERPROC), asi que el callback es llamado en case WM_TIMER
	UINT_PTR timerControl = SetTimer(g_hWnd, 1, T, (TIMERPROC)updateControllerState);

	g_szMessage[4][1024] = { 0 };
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
	KillTimer(g_hWnd, timerControl);

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


case WM_DESTROY:
{
PostQuitMessage(0);
KillTimer(g_hWnd, 1);
break;
}


}

return DefWindowProc(hWnd, msg, wParam, lParam);
}

void GeneraEventos(HIDXbox * Control){


	//Movimientos de joystis aqui
	POINT ptMouse;
	GetCursorPos(&ptMouse);


	RECT rectaux;
	HWND aux = GetDesktopWindow();
	GetWindowRect(aux, &rectaux);


	if (Control->LJX() != 0){
		ptMouse.x += Control->LJX() * 10;
	}

	if (Control->LJY() != 0){
		ptMouse.y -= Control->LJY() * 10;
	}


	if (Control->BD(XINPUT_GAMEPAD_LEFT_THUMB))	mouse_event(MOUSEEVENTF_LEFTDOWN, ptMouse.x, ptMouse.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_LEFT_THUMB))	mouse_event(MOUSEEVENTF_LEFTUP, ptMouse.x, ptMouse.y, 0, NULL);

	if (Control->BD((XINPUT_GAMEPAD_RIGHT_THUMB)))	mouse_event(MOUSEEVENTF_RIGHTDOWN, ptMouse.x, ptMouse.y, 0, NULL);
	if (Control->BU((XINPUT_GAMEPAD_RIGHT_THUMB)))	mouse_event(MOUSEEVENTF_RIGHTUP, ptMouse.x, ptMouse.y, 0, NULL);

	//Start y back
	if (Control->BD(XINPUT_GAMEPAD_BACK))keybd_event(VK_ESCAPE, 0x18, NULL, NULL);
	if (Control->BU(XINPUT_GAMEPAD_BACK))keybd_event(VK_ESCAPE, 0x18, KEYEVENTF_KEYUP, NULL);

	if (Control->BD(XINPUT_GAMEPAD_START))keybd_event(VK_RETURN, 0x18, NULL, NULL);
	if (Control->BU(XINPUT_GAMEPAD_START))keybd_event(VK_RETURN, 0x18, KEYEVENTF_KEYUP, NULL);



	//Botones X y B para inicio y fin
	if (Control->BD(XINPUT_GAMEPAD_X)){
		keybd_event(VK_HOME, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_X)){
		keybd_event(VK_HOME, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(XINPUT_GAMEPAD_B)){
		keybd_event(VK_END, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_B)){
		keybd_event(VK_END, 0x18, KEYEVENTF_KEYUP, NULL);
	}

	//Botones Y y A para Av pag Re pag
	if (Control->BD(XINPUT_GAMEPAD_Y)){
		keybd_event(VK_PRIOR, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_Y)){
		keybd_event(VK_PRIOR, 0x18, KEYEVENTF_KEYUP, NULL);
	}

	if (Control->BD(XINPUT_GAMEPAD_A)){
		keybd_event(VK_NEXT, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_A)){
		keybd_event(VK_NEXT, 0x18, KEYEVENTF_KEYUP, NULL);
	}




	//USO DEL PAD
	if (Control->BD(XINPUT_GAMEPAD_DPAD_LEFT)){
		keybd_event(VK_LEFT, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_DPAD_LEFT)){
		keybd_event(VK_LEFT, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(XINPUT_GAMEPAD_DPAD_RIGHT)){
		keybd_event(VK_RIGHT, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_DPAD_RIGHT)){
		keybd_event(VK_RIGHT, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(XINPUT_GAMEPAD_DPAD_UP)){
		keybd_event(VK_UP, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_DPAD_UP)){
		keybd_event(VK_UP, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(XINPUT_GAMEPAD_DPAD_DOWN)){
		keybd_event(VK_DOWN, 0x18, NULL, NULL);
	}
	if (Control->BU(XINPUT_GAMEPAD_DPAD_DOWN)){
		keybd_event(VK_DOWN, 0x18, KEYEVENTF_KEYUP, NULL);
	}



	//Rueda del ratón:
	if (Control->BD(XINPUT_GAMEPAD_LEFT_SHOULDER))  mouse_event(MOUSEEVENTF_WHEEL, ptMouse.x, ptMouse.y, 10, NULL);
	if (Control->BD(XINPUT_GAMEPAD_RIGHT_SHOULDER))  mouse_event(MOUSEEVENTF_WHEEL, ptMouse.x, ptMouse.y, -10, NULL);

	if (Control->BD(0xFFFFF)){
		Control->leftMotor(0.5f);
		Control->rightMotor(0.5f);
	}
	else if(Control->BU(0xFFFFF)) {
		Control->leftMotor(0.0f);
		Control->rightMotor(0.0f);
	}
	SetCursorPos(ptMouse.x, ptMouse.y);
	Control->writeController();
}
void GeneraEventos(HIDWii * Control){
	
	POINT ptMouse;
	GetCursorPos(&ptMouse);

	if (Control->BD(WIIMOTE_BUTTON_A)){
		mouse_event(MOUSEEVENTF_LEFTDOWN, ptMouse.x, ptMouse.y, 0, NULL);
	}
	if (Control->BU(WIIMOTE_BUTTON_A)){
		mouse_event(MOUSEEVENTF_LEFTUP, ptMouse.x, ptMouse.y, 0, NULL);
	}

	//Start y back
	if (Control->BD(WIIMOTE_BUTTON_HOME))keybd_event(VK_ESCAPE, 0x18, NULL, NULL);
	if (Control->BU(WIIMOTE_BUTTON_HOME))keybd_event(VK_ESCAPE, 0x18, KEYEVENTF_KEYUP, NULL);

	if (Control->BD(WIIMOTE_BUTTON_MINUS))keybd_event(VK_RETURN, 0x18, NULL, NULL);
	if (Control->BU(WIIMOTE_BUTTON_MINUS))keybd_event(VK_RETURN, 0x18, KEYEVENTF_KEYUP, NULL);



	//Botones X y B para inicio y fin
	if (Control->BD(WIIMOTE_BUTTON_ONE)){
		keybd_event(VK_HOME, 0x18, NULL, NULL);
	}
	if (Control->BU(WIIMOTE_BUTTON_ONE)){
		keybd_event(VK_HOME, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(WIIMOTE_BUTTON_TWO)){
		keybd_event(VK_END, 0x18, NULL, NULL);
	}
	if (Control->BU(WIIMOTE_BUTTON_TWO)){
		keybd_event(VK_END, 0x18, KEYEVENTF_KEYUP, NULL);
	}



	//USO DEL PAD
	if (Control->BD(WIIMOTE_BUTTON_LEFT)){
		keybd_event(VK_LEFT, 0x18, NULL, NULL);
	}
	if (Control->BU(WIIMOTE_BUTTON_LEFT)){
		keybd_event(VK_LEFT, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(WIIMOTE_BUTTON_RIGHT)){
		keybd_event(VK_RIGHT, 0x18, NULL, NULL);
	}
	if (Control->BU(WIIMOTE_BUTTON_RIGHT)){
		keybd_event(VK_RIGHT, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(WIIMOTE_BUTTON_UP)){
		keybd_event(VK_UP, 0x18, NULL, NULL);
	}
	if (Control->BU(WIIMOTE_BUTTON_UP)){
		keybd_event(VK_UP, 0x18, KEYEVENTF_KEYUP, NULL);
	}
	if (Control->BD(WIIMOTE_BUTTON_DOWN)){
		keybd_event(VK_DOWN, 0x18, NULL, NULL);
	}
	if (Control->BU(WIIMOTE_BUTTON_DOWN)){
		keybd_event(VK_DOWN, 0x18, KEYEVENTF_KEYUP, NULL);
	}


	if (Control->LJX() != 0){
		ptMouse.x += Control->LJX() * 10;
	}

	if (Control->LJY() != 0){
		ptMouse.y -= Control->LJY() * 10;
	}

	if (Control->BD(0xFFFFF)){
		Control->leftMotor(1);
		Control->rightMotor(1);
	}
	else if (Control->BU(0xFFFFF)) {
		Control->leftMotor(0.0f);
		Control->rightMotor(0.0f);
	}
	SetCursorPos(ptMouse.x, ptMouse.y);
	Control->writeController();

}





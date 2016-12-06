#pragma once
#ifndef _KEYBOARD_READER_H_
#define _KEYBOARD_READER_H_


#pragma once

#ifndef DLL_COMPILED
#define DLL_INTERNAL __declspec( dllexport )
#endif 

#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"
#include <string>
#include <list>

// Define all Colours you want
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF CYAN = RGB(00, 255, 255);
const COLORREF ORANGE = RGB(255, 165, 00);
const COLORREF PINK = RGB(255, 192, 203);
const COLORREF GREY = RGB(125, 125, 125);

//Background color for other keys
const COLORREF BACKGROUND = BLACK;
const COLORREF HIGHLIGHT = GREEN;

const long RAZER_KEYS[] = {
	0x0302, 0x0407, 0x0405, 0x0304, 0x0204, //A-E
	0x0305, 0x0306, 0x0307, 0x0209, 0x0308, //F-J
	0x0309, 0x030A, 0x0409, 0x0408, 0x020A, //K-O
	0x020B, 0x0202, 0x0205, 0x0303, 0x0206, //P-T
	0x0208, 0x0406, 0x0203, 0x0404, 0x0207, //U-Y 
	0x0403, //Z
	0x0102, 0x0103, 0x0104, 0x0105, 0x0106, //1-5
	0x0107, 0x0108, 0x0109, 0x010A, 0x010B  //6-0
};

#define KEYBOARD_DEVICES    1

//Class of your Chroma Implementation
class KeyboardReader
{
public:
	KeyboardReader();
	~KeyboardReader();
	BOOL Initialize();


	void ResetEffects(UINT DeviceType);
	//Define your methods here
	BOOL flash_word(std::string);
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE set_background_effect();
	BOOL IsDeviceConnected(RZDEVICEID DeviceId);

private:
	HMODULE m_ChromaSDKModule;
	
};

#endif

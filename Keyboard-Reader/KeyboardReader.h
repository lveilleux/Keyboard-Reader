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

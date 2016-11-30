#include "stdafx.h"
#include "KeyboardReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <list>

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif
using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace std;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

INIT Init = NULL;
UNINIT UnInit = NULL;
CREATEEFFECT CreateEffect = NULL;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
SETEFFECT SetEffect = NULL;
DELETEEFFECT DeleteEffect = NULL;
QUERYDEVICE QueryDevice = NULL;



BOOL KeyboardReader::IsDeviceConnected(RZDEVICEID DeviceId)
{
	if (QueryDevice != NULL)
	{
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		RZRESULT Result = QueryDevice(DeviceId, DeviceInfo);

		ASSERT(Result == RZRESULT_SUCCESS);

		return DeviceInfo.Connected;
	}

	return FALSE;
}


KeyboardReader::KeyboardReader() :m_ChromaSDKModule(NULL)
{
}
KeyboardReader::~KeyboardReader()
{
}

BOOL KeyboardReader::Initialize()
{
	if (m_ChromaSDKModule == NULL)
	{
		m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
		if (m_ChromaSDKModule == NULL)
		{
			ASSERT(GetLastError() == ERROR_SUCCESS);
			return FALSE;
		}
	}

	if (Init == NULL)
	{
		RZRESULT Result = RZRESULT_INVALID;
		Init = (INIT)GetProcAddress(m_ChromaSDKModule, "Init");
		if (Init)
		{
			Result = Init();
			if (Result == RZRESULT_SUCCESS)
			{
				CreateEffect = (CREATEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateEffect");
				CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect");
				SetEffect = (SETEFFECT)GetProcAddress(m_ChromaSDKModule, "SetEffect");
				DeleteEffect = (DELETEEFFECT)GetProcAddress(m_ChromaSDKModule, "DeleteEffect");
				QueryDevice = (QUERYDEVICE)GetProcAddress(m_ChromaSDKModule, "QueryDevice");

				if (CreateEffect &&
					CreateKeyboardEffect &&
					SetEffect &&
					DeleteEffect &&
					QueryDevice)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

void KeyboardReader::ResetEffects(UINT DeviceType) {
	if (CreateKeyboardEffect) {
		CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, NULL, NULL);
	}
}


BOOL KeyboardReader::flash_word(std::string word) {
	std::cout << " Result: " << word;
	RZRESULT Result_Keyboard;
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE keyboard_effect;
	keyboard_effect = this->set_background_effect();

	//Custom Color the Chroma Keyboard
	for (char c : word) {
		cout << c;
		//Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &keyboard_effect, NULL);
	}

	//return Result_Keyboard;
	return FALSE;
}

ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE KeyboardReader::set_background_effect() {
	//Creates a blank ChromaSDK Keyboard effect object to add in background effects
	//for the rest of the keyboard, which is returned to the calling function.
	//Currently onld does Static effects. Background color is found in Calculator.h
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE keyboard = {};

	for (UINT row = 0; row < ChromaSDK::Keyboard::MAX_ROW; row++) {
		for (UINT col = 0; col < ChromaSDK::Keyboard::MAX_COLUMN; col++) {
			keyboard.Color[row][col] = BACKGROUND;
		}
	}
	return keyboard;
}

int main() {

	KeyboardReader reader;
	BOOL test_for_init = reader.Initialize();
	cout << "Enter a filename: ";
	std::string fileName, line;
	cin >> fileName;
	cout << "\n";
	ifstream file(fileName, ios::in);

	if (file.is_open()) {
		while (getline(file, line)) {
			cout << line << '\n';
		}
		file.close();
	}
	else {
		cout << "Invalid filename/couldn't open file";
	}
	cin >> fileName;
	return 0;
}
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
	if (word.size() == 0) {
		return NULL;
	}
	RZRESULT Result_Keyboard;
	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE keyboard_effect;
	//Custom Color the Chroma Keyboard
	for (char& c : word) {
		long key;
		//Clear the keyboard in case of doubled keys (ie. "follow")
		keyboard_effect = this->set_background_effect();
		Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &keyboard_effect, NULL);
		Sleep(50);
		if (isalpha(c)) {
			c = toupper(c);
			key = RAZER_KEYS[c - 'A'];
			keyboard_effect.Color[key >> 8][key & 0xFF] = HIGHLIGHT;
		}
		else if (isdigit(c)) {
			key = RAZER_KEYS[c - '0' + 25]; //Offset for using the end of the RAZER_KEYS Array
			keyboard_effect.Color[key >> 8][key & 0xFF] = HIGHLIGHT;
		}
		else if (ispunct(c)) {
			key = RAZER_KEYS[c - ',' + 36]; //Offset for using end of RAZER_KEYS Array
			keyboard_effect.Color[key >> 8][key & 0xFF] = HIGHLIGHT;
		}
		Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &keyboard_effect, NULL);
		Sleep(250);
	}
	//Clear the keyboard
	keyboard_effect = this->set_background_effect();
	Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &keyboard_effect, NULL);
	Sleep(700);
	return Result_Keyboard;
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
	while (true) {
		cout << "Enter a filename (or EXIT to exit): ";
		std::string fileName, line;
		cin >> fileName;
		cout << "\n";
		if (fileName == "EXIT" || fileName == "") {
			return 0;
		}
		ifstream file(fileName, ios::in);

		if (file.is_open()) {
			while (getline(file, line)) {
				std::string word;
				cout << line << '\n';
				bool printedLast = false;
				for (char& c : line) {
					if (iscntrl(c) || isspace(c)) {
						reader.flash_word(word);
						word = "";
						printedLast = true;
					}
					else if (ispunct(c) && (c != '.' && c != ',' && c != '-')) {
						reader.flash_word(word);
						word = "";
						printedLast = true;
					}
					else {
						word += c;
						printedLast = false;
					}
				}
				if (!printedLast) {
					reader.flash_word(word);
				}
			}
			file.close();
		}
		else {
			cout << "Invalid filename/couldn't open file\n";
		}
	}
	return 0;
}
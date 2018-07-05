#include <string>
#include <windows.h>
#include <iostream>
#include "MinHook\MinHook.h"
#include "Util\Common.h"
#include "Util\Packet.h"
#include "Util\Hook.h"

#pragma comment(lib, "Ws2_32.lib")

#if defined _M_X64
#pragma comment(lib, "libMinHook-x64-v141-mdd.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mdd.lib")
#endif

void EjectDLL(HMODULE hModule)
{
	//std::cout << "Ejecting DLL after 0.5s" << std::endl;
	Sleep(500);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
};

int DisableHooks() {
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return 1;
	}

	// Uninitialize MinHook.
	if (MH_Uninitialize() != MH_OK)
	{
		return 1;
	}
	return 0;
}


DWORD WINAPI HookThread(LPVOID lpModule)
{
	util::CreateConsole();
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		util::Log("MH_Initialize failed");

	// Create a hook for Ws2_32.send
	if (MH_CreateHookApi(L"Ws2_32", "send", MySend, (LPVOID *)&psend) != MH_OK)
		util::Log("send hook failed");

	// Create a hook for Ws2_32.recv
	if (MH_CreateHookApi(L"Ws2_32", "recv", MyRecv, (LPVOID *)&precv) != MH_OK)
		util::Log("recv hook failed");

	// Enable the hooks.
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		util::Log("MH_EnableHook failed");
		return 1;
	}

	return 0;
}

std::string GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
		return ""; // error

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return ""; // error

	// Lock the handle to get the actual text pointer
	char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return ""; // error

	// Save text in a string class instance
	std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

int SendFromClipboard() {
	std::string data = GetClipboardText();
	std::string prefix = "0x";

	if (data.length() < 1)
	{
		return 1;
	}

	Packet* p = new Packet();

	for (unsigned int i = 0; i < data.size(); i += 3) {
		p->AddByte(std::stoi(prefix + data[i] + data[i + 1], nullptr, 0));
	}

	p->UpdatePacketSize();
	MySend(mySock, (char*)p->GetRawPacket(), p->GetRawSize(), 0);
	delete p;

	return 0;
}

DWORD WINAPI MainThread(LPVOID lpModule)
{

	while (1)
	{
		if (GetAsyncKeyState(VK_F1) & 1) {
			bLogSend = !bLogSend;
			printf("[bLogSend - %s]\n", bLogSend ? "true" : "false");
		}
		if (GetAsyncKeyState(VK_F2) & 1) {
			bLogRecv = !bLogRecv;
			printf("[bLogRecv - %s]\n", bLogRecv ? "true" : "false");
		}

		/*
		if (GetAsyncKeyState(VK_F8) & 1) {
			SendFromClipboard();
		}

		if (GetAsyncKeyState(VK_F10) & 1) {
			EjectDLL(reinterpret_cast<HMODULE>(lpModule));
		}
		*/

		Sleep(10);
	}

	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD_PTR ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, MainThread, hModule, NULL, NULL);
		CreateThread(NULL, 0, HookThread, hModule, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		DisableHooks();
		break;
	}
	return TRUE;
}
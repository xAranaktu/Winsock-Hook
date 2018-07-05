#include <windows.h>
#include <fcntl.h>
#include <stdio.h>
#include <io.h>
#include <string>
#include <fstream>
#include <commdlg.h>

#include "Util\Common.h"

void util::CreateConsole()
{
	if (!AllocConsole())
	{
		char buffer[1024] = { 0 };
		sprintf_s(buffer, "Failed to AllocConsole( ), GetLastError( ) = %d", GetLastError());
		MessageBox(HWND_DESKTOP, buffer, "Error", MB_OK);

		return;
	}

	auto lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	auto hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	auto fp = _fdopen(hConHandle, "w");

	freopen_s(&fp, "CONOUT$", "w", stdout);

	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

HMODULE util::GetCurrentModule()
{
	HMODULE hModule = NULL;

	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCTSTR>(GetCurrentModule),
		&hModule);

	return hModule;
}

void util::Log(const char *fmt, ...)
{
	if (!fmt)	return;

	char		text[4096];
	va_list		ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);

	std::ofstream logfile("C:\\Users\\Pawel\\Desktop\\log.txt", std::ios::app);
	if (logfile.is_open() && text) {
		logfile << text;
	}
	logfile.close();
}
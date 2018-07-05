#pragma once

namespace util
{
	void CreateConsole();
	HMODULE GetCurrentModule();
	void Log(const char *fmt, ...);
}
#include <windows.h>

#include "module.h"
#include "../error.h"

module::module(HMODULE hModule) : hModule(hModule) {}

std::string module::get_path(HANDLE hProcess) const {
	CHAR szModName[MAX_PATH];

	if (GetModuleFileNameExA(hProcess, hModule, szModName, sizeof(szModName) / sizeof(CHAR)) == 0)
		throw unknown_module_path();

	return std::string(szModName);
}

MODULEINFO module::get_module_information(HANDLE hProcess) const {
	MODULEINFO mi;

	if (GetModuleInformation(hProcess, hModule, &mi, sizeof(mi)) == 0)
		throw module_information_load_error();

	return mi;
}

std::vector<module> module::get_every_modules(HANDLE hProcess) {
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded) == 0) {
		ErrorExit((LPTSTR)"EnumProcessModules");
		throw module_load_error();
	}

	std::vector<module> modules;

	for (i=0; i<(cbNeeded/sizeof(HMODULE)); ++i)
		modules.push_back(module(hMods[i]));
		/*
		WCHAR szModName[MAX_PATH];
		if (GetModuleFileNameExW(handle.get(), hMods[i], szModName, sizeof(szModName) / sizeof(WCHAR))) {
			std::wstring wstrModName = szModName;
			std::wcout << wstrModName << '\n';
		}
		*/
	return modules;
}
#ifndef __MODULE_H__
#define __MODULE_H__

#include <windows.h>
#include <psapi.h>
#include <exception>
#include <string>
#include <vector>

class module_load_error : public std::exception {};
class unknown_module_path : public std::exception {};
class module_information_load_error : public std::exception {};

class module {
private:
	HMODULE _hModule;

public:
	module(HMODULE hModule);
	std::string get_path(HANDLE hProcess) const;
	MODULEINFO get_module_information(HANDLE hProcess) const;

	static std::vector<module> get_every_modules(HANDLE hProcess);
};

#endif
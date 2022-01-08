#include <windows.h>
#include <psapi.h>
#include <exception>
#include <memory>
#include <regex>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "../error.h"

void handle_deleter::operator()(HANDLE handle) { 
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
}

unique_HANDLE make_unique_HANDLE(HANDLE handle) {
	if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
		return nullptr;
	return unique_HANDLE(handle);
}

unique_HANDLE process::get_handle() const {
	unique_HANDLE handle = make_unique_HANDLE(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID));
	if (handle == nullptr)
		throw no_process_found();
	return std::move(handle);
}

std::vector<module> process::get_module() const {
	unique_HANDLE handle = get_handle();
	return module::get_every_modules(handle.get());
}

void process::initialize_path() {
	WCHAR szProcessPath[MAX_PATH];
	unique_HANDLE handle = get_handle();
	if (GetModuleFileNameExW(handle.get(), NULL, szProcessPath, sizeof(szProcessPath)/sizeof(WCHAR))) {
		std::wstring ws(szProcessPath);
		file_path = std::string(ws.begin(), ws.end());
	}
	else {
		throw unknown_process();
	}
}

void process::initialize_name() {
	std::string path = get_path();
	std::regex re(".+\\\\([^\\\\]+)$");
	std::smatch match;

	if (std::regex_match(path, match, re)) {
		try {
			file_name = match[1].str();
		}
		catch (const std::out_of_range &e) {
			throw weird_process_path();
		}
	}
	else
		throw weird_process_path();
}

process::process(DWORD PID) : PID(PID) {
	try {
		get_handle();
		initialize_path();
		initialize_name();
	}
	catch (const no_process_found &e) {
		throw e;
	}
}

DWORD process::get_PID() const {
	return PID;
}

std::string process::get_path() const {
	return file_path;
}

std::string process::get_name() const {
	return file_name;
}

MODULEINFO process::get_module_information() const {
	unique_HANDLE handle = get_handle();
	return get_module()[0].get_module_information(handle.get());
}

std::vector<BYTE> process::get_memory(LPCVOID base_address, SIZE_T size) const {
	SIZE_T numberOfBytesRead;
	std::unique_ptr<BYTE[]> buffer;
	std::vector<BYTE> memory;

	try {
		buffer = std::make_unique<BYTE[]>(size);
		unique_HANDLE handle = get_handle();

		if (ReadProcessMemory(handle.get(), base_address, buffer.get(), size, &numberOfBytesRead) == 0) {
			ErrorExit((LPTSTR)"ReadProcessMemory");
			throw inaccessible_process();
		}

		memory = std::vector<BYTE>(buffer.get(), buffer.get()+numberOfBytesRead);
	}
	catch (const std::bad_alloc &e) {
		throw too_large_to_allocate();
	}

	return memory;
}

std::vector<process> process::get_every_processes() {
	std::vector<process> processes;
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		throw process_load_error();

	cProcesses = cbNeeded / sizeof(DWORD);

	for (i=0; i<cProcesses; ++i)
		if (aProcesses[i] != 0) {
			try {
				processes.push_back(process(aProcesses[i]));
			}
			catch (const no_process_found &e) {
				// nop
			}
		}
	return processes;
}
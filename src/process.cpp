#include <windows.h>
#include <psapi.h>
#include <exception>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "process.h"
#include "error.h"

const char* no_process_found::what() const noexcept {
	return "failed to find process";
}

const char* unknown_process::what() const noexcept {
	return "failed to get process path";
}

const char* too_large_to_allocate::what() const noexcept {
	return "failed to allocate buffer";
}

const char* inaccessible_process::what() const noexcept {
	return "failed to access process memory";
}

const char* weird_process_path::what() const noexcept {
	return "failed to parse process path";
}

const char* process_load_error::what() const noexcept {
	return "failed to load processes";
}

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

std::vector<BYTE> process::get_memory(LPCVOID base_address, SIZE_T size) const {
	SIZE_T numberOfBytesRead;
	std::unique_ptr<BYTE[]> buffer;
	std::vector<BYTE> memory;

	try {
		buffer = std::make_unique<BYTE[]>(size);
		unique_HANDLE handle = get_handle();

		if (ReadProcessMemory(handle.get(), base_address, buffer.get(), size, &numberOfBytesRead) == 0)
			throw inaccessible_process();

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
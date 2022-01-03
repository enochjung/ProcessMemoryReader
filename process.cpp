#include "process.h"
#include <psapi.h>

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

process::process(DWORD PID) : PID(PID) {
	handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
	if (handle == NULL)
		throw no_process_found();
}

process::~process() {
	CloseHandle(handle);
}

std::string process::get_path() {
	if (file_name.empty()) {
		WCHAR szProcessPath[MAX_PATH];
		if (GetModuleFileNameExW(handle, NULL, szProcessPath, sizeof(szProcessPath)/sizeof(WCHAR))) {
			std::wstring ws(szProcessPath);
			file_name = std::string(ws.begin(), ws.end());
		}
		else
			throw unknown_process();
	}
	return file_name;
}

std::unique_ptr<std::vector<BYTE>> process::get_memory(LPCVOID base_address, SIZE_T size) {
	SIZE_T numberOfBytesRead;
	std::unique_ptr<BYTE[]> buffer;
	std::unique_ptr<std::vector<BYTE>> memory;

	try {
		buffer = std::make_unique<BYTE[]>(size);

		if (ReadProcessMemory(handle, base_address, buffer.get(), size, &numberOfBytesRead) == 0)
			throw inaccessible_process();

		memory = std::make_unique<std::vector<BYTE>>(buffer.get(), buffer.get()+numberOfBytesRead);
	}
	catch (const std::bad_alloc &e) {
		throw too_large_to_allocate();
	}

	return std::move(memory);
}
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <windows.h>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "module.h"

class no_process_found : public std::exception {};
class unknown_process : public std::exception {};
class too_large_to_allocate : public std::exception {};
class inaccessible_process : public std::exception {};
class weird_process_path : public std::exception {};
class process_load_error : public std::exception {};

struct handle_deleter {
	void operator()(HANDLE handle);
};

using unique_HANDLE = std::unique_ptr<void, handle_deleter>;
unique_HANDLE make_unique_HANDLE(HANDLE handle);

class process {
private:
	DWORD _PID;
	std::string _file_path;
	std::string _file_name;

	unique_HANDLE get_handle() const;
	std::vector<module> get_module() const;
	void initialize_path();
	void initialize_name();

public:
	process(DWORD PID);
	DWORD get_PID() const;
	std::string get_path() const;
	std::string get_name() const;
	MODULEINFO get_module_information() const;
	std::vector<BYTE> get_memory(LPCVOID base_address, SIZE_T size) const;

	static std::vector<process> get_every_processes();
};

#endif
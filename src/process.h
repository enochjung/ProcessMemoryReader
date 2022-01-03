#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <windows.h>
#include <exception>
#include <memory>
#include <string>
#include <vector>

class no_process_found : public std::exception {
public:
	virtual const char* what() const noexcept;
};

class unknown_process : public std::exception {
public:
	virtual const char* what() const noexcept;
};

class too_large_to_allocate : public std::exception {
public:
	virtual const char* what() const noexcept;
};

class inaccessible_process : public std::exception {
public:
	virtual const char* what() const noexcept;
};

class weird_process_path : public std::exception {
public:
	virtual const char* what() const noexcept;
};

class process_load_error : public std::exception {
public:
	virtual const char* what() const noexcept;
};

struct handle_deleter {
	void operator()(HANDLE handle);
};

using unique_HANDLE = std::unique_ptr<void, handle_deleter>;
unique_HANDLE make_unique_HANDLE(HANDLE handle);

class process {
private:
	DWORD PID;
	std::string file_path;
	std::string file_name;
	unique_HANDLE get_handle() const;
	void initialize_path();
	void initialize_name();

public:
	process(DWORD PID);
	DWORD get_PID() const;
	std::string get_path() const;
	std::string get_name() const;
	std::vector<BYTE> get_memory(LPCVOID base_address, SIZE_T size) const;

	static std::vector<process> get_every_processes();
};

#endif
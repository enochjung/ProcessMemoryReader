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

class process {
private:
	DWORD PID;
	HANDLE handle;
	std::string file_name;

public:
	process(DWORD PID);
	~process();
	std::string get_path();
	std::unique_ptr<std::vector<BYTE>> get_memory(LPCVOID base_address, SIZE_T size);
};

#endif
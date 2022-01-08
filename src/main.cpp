#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include <iostream>

#include "process/process.h"

int main() {
	std::vector<process> processes = process::get_every_processes();

	std::sort(processes.begin(), processes.end(), [](const process &lhs, const process &rhs) -> bool {
		std::string lstr = lhs.get_name();
		std::string rstr = rhs.get_name();
		std::transform(lstr.begin(), lstr.end(), lstr.begin(), ::tolower);
		std::transform(rstr.begin(), rstr.end(), rstr.begin(), ::tolower);
		return lstr < rstr;
	});

	for (process p : processes) {
		try {
			std::cout << p.get_PID() << '\t' << p.get_name() << '\n';
		}
		catch (const unknown_process &e) {
			std::cout << "failed to get file name\n";
		}
	}

	int pid;
	std::cout << "\ninput pid > ";
	std::cin >> pid;
	for (process p : processes) {
		if (p.get_PID() == pid) {
			MODULEINFO mi = p.get_module_information();
			std::cout << "base address : " << mi.lpBaseOfDll << "\n"
					  << "entry point : " << mi.EntryPoint << "\n"
					  << "size of image : " << mi.SizeOfImage << "\n";

			std::vector<BYTE> memory = p.get_memory(mi.lpBaseOfDll, mi.SizeOfImage);
			std::cout << "\nsize : " << memory.size() << "\n";
			for (BYTE b : memory) {
				std::cout << std::hex << (int)b;
			}
		}
	}

	return 0;
}
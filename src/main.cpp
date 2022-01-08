#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include <iostream>

#include "process/process.h"

int main()
{
	std::vector<process> processes = process::get_every_processes();

	std::sort(processes.begin(), processes.end(), [](const process &lhs, const process &rhs) -> bool {
		return lhs.get_name() < rhs.get_name();
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
	std::cout << "\ninput pid >";
	std::cin >> pid;
	for (process p : processes) {
		if (p.get_PID() == pid) {
			MODULEINFO mi = p.get_module_information();
			std::cout << "base address : " << mi.lpBaseOfDll << "\n";
		}
	}

	return 0;
}
#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include <iostream>

#include "process.h"

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

	return 0;
}
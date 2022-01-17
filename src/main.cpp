#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include <iostream>
#include <optional>

#include "process/process.h"
#include "search/searcher.h"

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

	auto find_process_by_pid = [](std::vector<process> &processes, int pid) -> std::optional<process> {
		for (process &p : processes)
			if (p.get_PID() == pid)
				return p;
		return std::nullopt;
	};

	std::optional<process> selected_process = find_process_by_pid(processes, pid);
	if (selected_process.has_value()) {
		process p = selected_process.value();
		searcher<int> s(p);

		auto print_memory = [&s]() -> void {
			std::vector<memory_pair<int>> m = s.get_memory();
			if (m.size() > 400)
				std::cout << m.size() << " items searched.\n";
			else if (m.size() == 0)
				std::cout << "there is no item.\n";
			else {
				for (memory_pair<int>& mp : m)
					std::cout << mp.address << "\t: " << mp.value << "\n";
			}
		};

		print_memory();
		while (true) {
			int value;

			std::cout << "input value > ";
			std::cin >> value;
			s.search_next(value);
			print_memory();
		}


		/* print all memory
		MODULEINFO mi = p.get_module_information();
		std::cout << "base address : " << mi.lpBaseOfDll << "\n"
				  << "entry point : " << mi.EntryPoint << "\n"
				  << "size of image : " << mi.SizeOfImage << "\n";

		std::vector<BYTE> memory = p.get_memory(mi.lpBaseOfDll, mi.SizeOfImage);
		std::cout << "\nsize : " << memory.size() << "\n";
		for (BYTE b : memory) {
			std::cout << std::hex << (int)b;
		}
		*/
	}
	else {
		std::cout << "wrong pid\n";
	}

	return 0;
}
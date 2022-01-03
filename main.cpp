#include <windows.h>
#include <psapi.h>
#include <iostream>

#include "process.h"

int main()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return 1;
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	for (i=0; i<cProcesses; ++i) {
		if (aProcesses[i] != 0 ) {
			try {
				process p(aProcesses[i]);
				std::cout << p.get_path() << '\n';
			}
			catch (const no_process_found &e) {
			}
			catch (const unknown_process &e) {
				std::cout << "failed to get file name\n";
			}
		}
	}

	return 0;
}
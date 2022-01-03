/*
void ErrorExit(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw); 
}


void PrintProcessNameAndID(DWORD processID) {

	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	TCHAR szProcessName[MAX_PATH];
	unsigned int i;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	if (hProcess == NULL) {
		return;
	}

	if (WaitForInputIdle(hProcess, INFINITE)) {
		//ErrorExit((LPTSTR)TEXT("WaitForInputIdle"));
		_tprintf(TEXT("(NC) "));
	}
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {

		for (i=0; i<(cbNeeded / sizeof(HMODULE)); ++i) {
			TCHAR szModName[MAX_PATH];

			if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName)/sizeof(TCHAR))) {
				_tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
			}
		}
	}
	if (GetModuleFileNameEx(hProcess, NULL, szProcessName, sizeof(szProcessName)/sizeof(TCHAR))) {
		_tprintf(TEXT("%s\n"), szProcessName);
	}
	else {
		ErrorExit((LPTSTR)TEXT("GetModuleFileNameEx"));
		//_tprintf(TEXT("EnumProcessModules Error. PID:%u, HANDLE:%u\n"), processID, hProcess);
	}

	CloseHandle(hProcess);
}
*/

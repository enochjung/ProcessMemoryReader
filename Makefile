CPP_VERSION_OPTION=-std=c++17
PSAPI_OPTION=-lPsapi -DPSAPI_VERSION=1

LINK_OPTION=$(PSAPI_OPTION) $(CPP_VERSION_OPTION)

bin/pmr.exe: src/main.cpp bin/process.o bin/error.o
	g++ src/main.cpp bin/process.o bin/error.o -o bin/pmr.exe $(LINK_OPTION)

bin/process.o: src/process.h src/process.cpp
	g++ src/process.cpp -c $(LINK_OPTION) -o bin/process.o

bin/error.o: src/error.h src/error.cpp
	g++ src/error.cpp -c $(LINK_OPTION) -o bin/error.o
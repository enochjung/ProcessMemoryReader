CPP_VERSION_OPTION=-std=c++17
PSAPI_OPTION=-lPsapi -DPSAPI_VERSION=1

LINK_OPTION=$(PSAPI_OPTION) $(CPP_VERSION_OPTION)

bin/pmr.exe: src/main.cpp bin/process.o bin/module.o bin/error.o
	g++ src/main.cpp bin/process.o bin/module.o bin/error.o -o bin/pmr.exe $(LINK_OPTION)

bin/module.o: src/process/module.h src/process/module.cpp
	g++ src/process/module.cpp -c $(LINK_OPTION) -o bin/module.o

bin/process.o: src/process/process.h src/process/process.cpp
	g++ src/process/process.cpp -c $(LINK_OPTION) -o bin/process.o

bin/error.o: src/error.h src/error.cpp
	g++ src/error.cpp -c $(LINK_OPTION) -o bin/error.o
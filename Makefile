CPP_VERSION_OPTION=-std=c++17
PSAPI_OPTION=-lPsapi -DPSAPI_VERSION=1

LINK_OPTION=$(PSAPI_OPTION) $(CPP_VERSION_OPTION)

all: src/main.cpp bin/process.o
	g++ src/main.cpp bin/process.o -o bin/pmr.exe $(LINK_OPTION)

bin/process.o: src/process.h src/process.cpp
	g++ src/process.cpp -c $(LINK_OPTION) -o bin/process.o
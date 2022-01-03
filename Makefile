CPP_VERSION_OPTION=-std=c++17
PSAPI_OPTION=-lPsapi -DPSAPI_VERSION=1

LINK_OPTION=$(PSAPI_OPTION) $(CPP_VERSION_OPTION)

all: main.cpp process.o
	g++ main.cpp process.o -o pmr.exe $(LINK_OPTION)

process.o: process.h process.cpp
	g++ process.cpp -c $(LINK_OPTION)
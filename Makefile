LINK_OPTION=-lPsapi -DPSAPI_VERSION=1

all: main.cpp
	g++ main.cpp -o pmr.exe $(LINK_OPTION)
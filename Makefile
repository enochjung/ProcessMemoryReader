CXX=g++
CXXFLAGS=-std=c++17 -m64
PSAPIFLAGS=-lPsapi -DPSAPI_VERSION=1
FLAGS=$(PSAPIFLAGS) $(CXXFLAGS)
TARGET=pmr.exe

bin/$(TARGET): src/main.cpp bin/process.o bin/module.o bin/error.o
	g++ src/main.cpp bin/process.o bin/module.o bin/error.o $(FLAGS) -o bin/$(TARGET)

bin/module.o: src/process/module.h src/process/module.cpp
	g++ src/process/module.cpp -c $(FLAGS) -o bin/module.o

bin/process.o: src/process/process.h src/process/process.cpp
	g++ src/process/process.cpp -c $(FLAGS) -o bin/process.o

bin/error.o: src/error.h src/error.cpp
	g++ src/error.cpp -c $(FLAGS) -o bin/error.o

clean:
	del bin\*.o
	del bin\$(TARGET)
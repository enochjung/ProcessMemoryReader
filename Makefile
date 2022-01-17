CXX=g++
CXXFLAGS=-std=c++17 -m64
PSAPIFLAGS=-lPsapi -DPSAPI_VERSION=1
FLAGS=$(PSAPIFLAGS) $(CXXFLAGS)
TARGET=pmr.exe

bin/$(TARGET): src/main.cpp bin/process.o bin/module.o bin/error.o bin/searcher.o
	$(CXX) src/main.cpp bin/process.o bin/module.o bin/error.o bin/searcher.o $(FLAGS) -o bin/$(TARGET)

bin/module.o: src/process/module.h src/process/module.cpp
	$(CXX) src/process/module.cpp -c $(FLAGS) -o bin/module.o

bin/process.o: src/process/process.h src/process/process.cpp
	$(CXX) src/process/process.cpp -c $(FLAGS) -o bin/process.o

bin/searcher.o: src/search/searcher.h src/search/searcher.cpp
	$(CXX) src/search/searcher.cpp -c $(FLAGS) -o bin/searcher.o

bin/error.o: src/error.h src/error.cpp
	$(CXX) src/error.cpp -c $(FLAGS) -o bin/error.o

clean:
	del bin\*.o
	del bin\$(TARGET)
#ifndef __SEARCHER_H__
#define __SEARCHER_H__

#include <windows.h>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "../process/process.h"

//class exception_name : public std::exception {};

template<typename T>
struct memory_pair {
	int address;
	T value;
};

template<typename T>
class searcher {
private:
	const process _process;
	std::vector<int> _memory_index;

	std::vector<T> get_every_memory() const;

public:
	searcher(process process);
	void search_next(T value);
	std::vector<memory_pair<T>> get_memory() const;
};

template class searcher<int>;

#endif
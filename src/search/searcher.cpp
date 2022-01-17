#include "searcher.h"

template<typename T>
std::vector<T> searcher<T>::get_every_memory() const {
	const int size = sizeof(T);
	const MODULEINFO mi = _process.get_module_information();
	const std::vector<BYTE> memory_BYTE = _process.get_memory(mi.lpBaseOfDll, mi.SizeOfImage);

	std::vector<T> memory_T((memory_BYTE.size()+size-1) / size);
	memcpy(&memory_T[0], &memory_BYTE[0], memory_BYTE.size());

	return memory_T;
}

template<typename T>
searcher<T>::searcher(process process) : _process(process) {
	const int size = sizeof(T);
	const std::vector<T> memory_T = get_every_memory();

	_memory_index.resize(memory_T.size());
	for (int i=0; i<_memory_index.size(); ++i)
		_memory_index[i] = i;
}

template<typename T>
void searcher<T>::search_next(T value) {
	const int size = sizeof(T);
	const std::vector<T> memory_T = get_every_memory();
	std::vector<int> new_memory_index;

	for (int idx : _memory_index) {
		if (memory_T[idx] == value)
			new_memory_index.push_back(idx);
	}

	_memory_index = new_memory_index;
}

template<typename T>
std::vector<memory_pair<T>> searcher<T>::get_memory() const {
	const int size = sizeof(T);
	const std::vector<T> memory_T = get_every_memory();
	std::vector<memory_pair<T>> memory;

	for (int idx : _memory_index)
		memory.push_back({idx*size, memory_T[idx]});
	return memory;
}
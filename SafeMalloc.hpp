#pragma once
#include <list>

class SafeMalloc {

private:
	std::list<void*> to_free;
public:
	SafeMalloc(){}

	template<typename T>
	T* malloc(size_t size){
		void* tf = ::malloc(sizeof(T) * size);
		to_free.push_back(tf);
		return (T*) tf;
	}

	virtual ~SafeMalloc(){
		for (void *mem : to_free) free(mem);
	}

};

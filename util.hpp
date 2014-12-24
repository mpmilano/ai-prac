#pragma once
#include <memory>

template<typename C, typename L>
auto strengthen(const L &l) -> C{
	C c;
	for (auto &e : l) c.push_back(e.lock());
	return c;
}

#pragma once

template<int n_p, typename T>
class NHist{
	static constexpr int n = n_p + 1;
	static_assert(n >= 1,"Error: use this when you have at least two generations.");

	std::array<T, n> hist;
	int currpos = 0;

public:
	auto& curr(){
		return hist[currpos];
	}

	NHist& advance(bool clear = true){
		++currpos;
		if (currpos == n) currpos = 0;
		if (clear) curr().clear();
		return *this;
	}
	auto& prev(int g = 1){
		auto r = currpos - g;
		if (r < 0) return hist[g + r];
		else return hist[r];
	}

};

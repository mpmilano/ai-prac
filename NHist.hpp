#pragma once

//why does this std::array seem to have exclusively identical objects in it?
//hacking around the problem for now.  Still, semantically disturbing. 
template<int n_p, typename T>
class NHist{
	static constexpr int n = n_p;
	static_assert(n >= 1,"Error: use this when you have at least two generations.");

	T curr_T;
	std::array<T, n> hist;
	int currpos = 0;

public:

	NHist(){
		assert(&(prev()) != &(curr()));
	}
	
	auto& curr(){
		return curr_T;
	}

	NHist& advance(bool clear = true){
		++currpos;
		if (currpos == n) currpos = 0;
		hist[currpos] = curr_T;
		assert(prev().size() != 0);
		assert(&(prev()) != &(curr()));
		if (clear) curr().clear();
		assert(prev().size() != 0);
		return *this;
	}
	
	auto& prev(int g_p = 1){
		auto g = g_p - 1;
		auto r = currpos - g;
		auto &ret = ((r < 0) ? hist[g + r] : hist[r]);
		return ret;
	}

};

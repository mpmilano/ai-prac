#pragma once

template<int n, typename T>
class NHist{

	std::array<n,T> hist;
	int currpos = 0;

public:
	auto& curr(){
		return hist[currpos];
	}

	void advance(){
		++currpos;
		if (currpos == n) currpos = 0;
	}
	auto& prev(int n = 1){
		auto r = currpos - n;
		if (r < 0) return hist[n + r];
		else return hist[r];
	}

};

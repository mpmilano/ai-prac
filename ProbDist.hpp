#pragma once
#include <list>
#include <cstdlib>
#include <array>

template<typename LE>
class ProbDist{
	typedef typename std::decay<LE>::type::value_type T;
	static constexpr int granularity = 10000;
	std::array<const T*, granularity> source;
	
public:
	
	ProbDist(const LE &objs, const std::list<double> &probs){
		int i = 0;
		double accp = 0;
		auto it = probs.begin();
		for (const auto &e : objs){
			const auto &p = *it;
			accp += p;
			for (; (i < accp * granularity); ++i){
				source[i] = &e;
			}
			++it;
		}
	}
	const T& choose(const T& exclude){
		auto* cand = &choose();
		while (cand == &exclude) cand = &choose();
		return *cand;
	}

	const T& choose(){
		return *source[rand()%granularity];
	}

	template<typename C>
	static C normalize(double v, const C& c){
		C nc;
		typename std::decay<decltype(c.back())>::type total = 0;
		for (auto &e : c) total+=e;
		for (auto &e : c) nc.push_back(v * e / total);
		return nc;
	}
	
	
};

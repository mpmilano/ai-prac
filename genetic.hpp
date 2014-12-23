#pragma once
#include <list>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>
#include <limits>
#include <memory>
#include "ProbDist.hpp"
#include "NHist.hpp"


template<typename F, typename G, typename H, typename I>
auto genetic(F generate_f, int k, G rank_f, H done_f, I mutate_f) {

	constexpr int limit = std::numeric_limits<decltype(limit)>::max();

	
	typedef decltype(generate_f()) vec;
	typedef typename vec::value_type T;

	std::function<bool (const vec& )> done = done_f;
	std::function<vec () > generate = generate_f;
	std::function<double ( const vec&) > rank = rank_f;
	std::function<T (const T&)> mutate_e = mutate_f;
	
	auto mutate = [&](const auto &v){
		auto r = v;
		for (auto e = r.begin(); e != r.end(); ++e)
			*e = mutate_e(*e);
		return r;
	};

	{
		
	auto gens_p = std::make_unique<NHist<1,std::list<vec> > >();
	auto& gens = *gens_p;
	for (int i = 0; i < k; ++i) gens.curr().push_back(generate());

	gens.advance();
	
	for (typename std::decay<decltype(limit)>::type asdfjsaidf = 0; asdfjsaidf < limit; ++asdfjsaidf){
		{
			const auto& parents = gens.prev();
			assert(parents.size() != 0);

			//check to see if we're done
			for (auto &v : parents) if (done(v)) {
					std::cerr << "DONE!!!" << std::endl;
					return v;
				}
			
			//score current breeding members
			std::list<double> scores;
			for (auto &v : parents) scores.push_back(rank(v));
			auto scores2 = ProbDist<decltype(parents)>::normalize(1.0,scores);
			ProbDist<decltype(parents)> pd(parents, scores2);
			
			
			//assign breeding couples
			std::list<std::pair<vec, vec > > couples;
			for (decltype(parents.size()) i = 0; i < parents.size() / 2; ++i){
				auto first = pd.choose();
				auto second = pd.choose(first);
				couples.emplace_back(first, second);
			}
			auto &nextGen = gens.curr();

			//make new generation
			for (auto &p : couples){
				//pick split position. 
				decltype(p.first.size()) point = rand() % (p.first.size());
				vec a;
				vec b;
				decltype(point) i = 0;
				//take first part of first + second vectors
				for (; i < point; ++i){
					a[i] = p.first.at(i);
					b[i] = p.second.at(i);
				}
				//take second part of first + second vectors
				for (; i < p.first.size(); ++i){
					a[i] = p.second.at(i);
					b[i] = p.first.at(i);
				}					
				
				assert(i == p.first.size() && i == p.second.size());
				
				//potentially mutate each member of next generation
				nextGen.push_back(mutate(a));
				nextGen.push_back(mutate(b));
			}
		}
		//return recr(gens.advance());
		gens.advance();	
	}
	//failed!
	assert(false && "failed!");
	return gens.curr().front();
	}
}

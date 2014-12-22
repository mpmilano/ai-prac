#include <list>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>
#include <limits>
#include <memory>
#include "ProbDist.hpp"
#include "NHist.hpp"

using namespace std;

constexpr int limit = std::numeric_limits<decltype(limit)>::max();

template<typename T>
void print(T &){
	std::cout << "I don't know how to print this!" << std::endl;
}

template<typename F, typename G, typename H, typename I>
auto genetic(F generate_f, int k, G rank_f, H done_f, I mutate_f) {
	typedef decltype(generate_f()) vec;
	typedef typename vec::value_type T;

	function<bool (const vec& )> done = done_f;
	function<vec () > generate = generate_f;
	function<double ( const vec&) > rank = rank_f;
	function<T (const T&)> mutate_e = mutate_f;
	
	auto mutate = [&](const auto &v){
		auto r = v;
		for (auto e = r.begin(); e != r.end(); ++e)
			*e = mutate_e(*e);
		return r;
	};
	
	auto gens_p = std::make_unique<NHist<1,list<vec> > >();
	auto& gens = *gens_p;
	for (int i = 0; i < k; ++i) gens.curr().push_back(generate());

	gens.advance();
	
	for (typename std::decay<decltype(limit)>::type asdfjsaidf = 0; asdfjsaidf < limit; ++asdfjsaidf){
		{
			const auto& parents = gens.prev();
			assert(parents.size() != 0);

			//check to see if we're done
			for (auto &v : parents) if (done(v)) return v;
			
			//score current breeding members
			list<double> scores;
			for (auto &v : parents) scores.push_back(rank(v));
			auto scores2 = ProbDist<decltype(parents)>::normalize(1.0,scores);
			ProbDist<decltype(parents)> pd(parents, scores2);
			
			
			//assign breeding couples
			list<pair<vec, vec > > couples;
			for (decltype(parents.size()) i = 0; i < parents.size() / 2; ++i){
				auto first = pd.choose();
				auto second = pd.choose(first);
				couples.emplace_back(first, second);
			}
			auto &nextGen = gens.curr();
			static_assert(is_same<typename decay<decltype(nextGen)>::type, list<vec> >::value,
						  "Why aren't these the same?");
			
			
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

//8 queens on an 8x8 board

namespace rooks{
	constexpr int size = 8;
	typedef int rook;
	typedef std::array<rook,size> board;

	double rank (const board &b){
		double score = 0;
		std::array<bool, size> hits;
		for (int i  = 0; i < size; ++i) hits[i] = false;
		for (auto &col : b){
			if (hits[col]) ++score;
			else hits[col] = true;
		}

		return size - score;
	}

	void print(const board &b){
		std::cout << "<";
		for (auto &e : b) std::cout << e << ",";
		std::cout << ">" << std::endl;
	}

	bool done(const board &b){
		return rank(b) == size;
	}

	rook mutate(const rook &r){
		if (rand() % 12 == 3) return (r + 1) % size;
		else return r;
	}

	board generate(){
		std::array<rook,size> ret;
		for (int i = 0; i < size; ++i)
			ret[i] = rand() % size;
		return ret;
	}
}
template<>
void print(const list<rooks::board>& v){

	std::cout << std::endl;
	for (const auto &e : v) rooks::done(e);
	std::cout << std::endl;

}

int main(){

	srand(12);

	//generate, k, rank, done, mutate
	auto result = genetic(rooks::generate, 8, rooks::rank, rooks::done, rooks::mutate);
	rooks::print(result);
		
}

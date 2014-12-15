#include <list>
#include <vector>
#include <functional>
#include "ProbDist.hpp"
#include "NHist.hpp"

using namespace std;

template<typename F, typename G, typename H, typename I>
auto genetic(F generate_f, int k, G rank_f, H done_f, I mutate_f){
	typedef typename decltype(generate_f())::value_type T;
	typedef decltype(generate_f()) vec;
	function<bool (const vec&)> done = done_f;
	function<vec () > generate = generate_f;
	function<double (const vec&) > rank = rank_f;
	function<T (const T&)> mutate_e = mutate_f;
	auto mutate = [&](const auto &v){
		typename std::decay<decltype(v)>::type r;
		for (auto &e : v)
			if (rand() %100 == 1) r.push_back(mutate_e(e));
			else r.push_back(e);
		return r;
	};
	
	NHist<1,list<vec> > parents;
	for (int i = 0; i < k; ++i) parents.curr().push_back(generate());
	std::function<const vec& (decltype(parents)&)> recr;
	recr = [&recr, &done, &rank, &mutate](auto& gens) -> const vec&{
		const auto& parents = gens.prev();
		for (auto &v : parents) if (done(v)) return v;
		list<double> scores;
		for (auto &v : parents) scores.push_back(rank(v));
		auto scores2 = ProbDist<decltype(parents)>::normalize(1.0,scores);
		ProbDist<decltype(parents)> pd(parents, scores2);
		list<pair<vec, vec > > couples;
		for (decltype(parents.size()) i = 0; i < parents.size() / 2; ++i){
			auto first = pd.choose();
			auto second = pd.choose(first);
			couples.emplace_back(first, second);
		}
		auto &nextGen = gens.curr();
		static_assert(is_same<typename decay<decltype(nextGen)>::type, list<vec> >::value,
					  "Why aren't these the same?");
		for (auto &p : couples){
			decltype(p.first.size()) point = rand() % (p.first.size());
			vec a;
			vec b;
			decltype(point) i = 0;
			for (; i < point; ++i){
				a[i] = p.first.at(i);
				b[i] = p.second.at(i);
			}
			for (; i < p.first.size(); ++i){
				a[i] = p.second.at(i);
				b[i] = p.first.at(i);
			}
			nextGen.push_back(mutate(a));
			nextGen.push_back(mutate(b));
		}
		return recr(gens.advance());
	};
	return recr(parents.advance());
	
}

constexpr int size = 8;

int main(){

	auto summ = [](const std::vector<int> &c ) -> double{
			int acc = 0;
			for (auto &i : c) acc+=i;
			return acc;
	};
	
	genetic([](){
			std::vector<int> ret(size);
			return ret;
		},
		8,
		summ,
		[&summ](const std::vector<int> &c ){
			return summ(c) > 18;
		}, [](auto e){return e;});
		
}

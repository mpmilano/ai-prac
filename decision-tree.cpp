#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <cmath>
#include <set>
#include <memory>
#include "tree.h"

using namespace std;


namespace training{
	typedef list<shared_ptr<unique_ptr <pair<vector<int>, bool> > > > t;
	typedef t::value_type v;
	
	const auto& at(const t::value_type& p, int ind){
		return (*p)->first.at(ind);
	}

	bool val(const t::value_type& p){
		return (*p)->second;
	}

	auto vsize(const t& t){
		return (*t.front())->first.size();
	}

}

namespace effectiveness {
	typedef vector<map<int, pair<set<training::v>, set<training::v> > > > t;

	auto incrpos(t::value_type &p, int ind, training::v v){
		return p[ind].first.insert(v);
	}

	auto incrneg(t::value_type &p, int ind, training::v v){
		return p[ind].second.insert(v);
	}

	int size(const t::value_type &p){
		return p.size();
	}

	auto &map(t::value_type &p){
		return p;
	}

	const auto &map(const t::value_type &p){
		return p;
	}


	int numpos(t::value_type::mapped_type &p){
		decltype(p.first) nf;
		for (auto &e : p.first) if (*e) nf.insert(e);
		p.first = nf;
		return p.first.size();
	}

	void nukepos(t::value_type::mapped_type &p){
		for (auto &e : p.first) if (*e) e->reset();
	}


	int numneg(t::value_type::mapped_type &p){
		decltype(p.second) nf;
		for (auto &e : p.second) if (*e) nf.insert(e);
		p.second = nf;
		return p.second.size();
	}

	void nukeneg(t::value_type::mapped_type &p){
		for (auto &e : p.second) if (*e) e->reset();
	}

	void nuke(t::value_type::mapped_type &p){
		nukepos(p);
		nukeneg(p);
	}

	
	auto& at(t &t, int i) {return t.at(i); }
}

namespace diagram{
	typedef int t;
	typedef int label;
	constexpr int other = -1;
	constexpr int YES = -3;
	constexpr int NO = -2;
	constexpr int begin = -1;
}

template<typename T1, typename T2, typename T3>
void build_diagram (T1 next, T2 pos, T3& eff) {

	using namespace std::placeholders;
	
	auto &cand = eff.at(pos);
	//make node corresponding to "cand", hook it to dangling -->
	auto &node = Tree::hookin(next,(int) pos);
	
	bool other_case = false;
	for (auto &p : cand){
		if (effectiveness::numpos(p.second) == 0){
			//add p.first -> "NO"
			Tree::addChild(node,p.first,diagram::NO);
			//clear vectors which have been satisfied
			effectiveness::nuke(p.second);
		}
		else if (effectiveness::numneg(p.second) == 0){
			//add p.first -> "YES"
			Tree::addChild(node, p.first,diagram::YES);
			//clear vectors which have been satisfied
			effectiveness::nuke(p.second);
		}
		else other_case = true;
	}
	if (other_case){
		//add dangling else -->
		auto posn = pos + 1;
		if (posn < eff.size()){
			build_diagram(Tree::dangle(node,diagram::other), posn,eff);
		}
	}
}

auto learn(training::t tning){
	auto size = training::vsize(tning);
	effectiveness::t eff;
	int totalyes = 0;
	for (auto &ex : tning) totalyes += training::val(ex);
	for (decltype(size) pos = 0; pos < size; ++pos){
		auto &m = effectiveness::at(eff,pos);
		for (auto &ex : tning){
			//record number of successes with this value
			if (training::val(ex)) effectiveness::incrpos(m, training::at(ex,pos), ex );
			else effectiveness::incrneg (m,training::at(ex,pos), ex );
		}
	}

	auto score = [totalyes](auto &a){
		auto expected = (double) totalyes / effectiveness::size(a);
		decltype(expected) delta = 0;
		for (auto &p : effectiveness::map(a))
			delta += fabs(effectiveness::numpos(p.second) - expected);
		return delta;
	};

	std::sort(eff.begin(), eff.end(),
			  [&totalyes,&score](auto& a, auto& b){
				  return score(a) > score(b);
			  }
		);

	Tree::t<diagram::t, diagram::label> tree;

	tree.val = diagram::begin;
	//create dangling begin -->
	auto next = Tree::dangle(tree,diagram::other);

	build_diagram(next, (decltype(eff.size())) 0, eff );
}

int main(){}

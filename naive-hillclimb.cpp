#include "Node.hpp"
#include "util.hpp"
#include <iostream>
#include <cmath>

template <typename N, typename F, typename G>
N hillclimb(const N& initial, F value, G neighbors ){
	auto cur = value(initial);
	auto next = initial;
	bool any = false;
	for (auto &n : neighbors(initial)) {
		if (value(n) > cur){
			any = true;
			next = n;
			cur = value(next);
		}
	}
	if (any) return hillclimb(next, value, neighbors);
	else return initial;
}

enum Queen {
	one, two, three, four
};

typedef	std::map<Queen, std::pair<int, int> > board;

void queens(){
	auto queen_l = {one, two, three, four};
	board b;
	b[one] = std::make_pair(2,3);
	b[two] = std::make_pair(0,3);
	b[three] = std::make_pair(1,6);
	b[four] = std::make_pair(2,5);
	auto final = hillclimb(b, [](const board &b){
			auto hits = [&](const auto &a, const auto &c) -> int{
				if (a == c) return std::numeric_limits<int>::min();
				return -1 * (b.at(a).first == b.at(c).first
							 || b.at(a).second == b.at(c).second
							 || std::abs(b.at(c).first - b.at(a).first) == std::abs(b.at(c).second - b.at(a).second));
			};
			return hits(one,two) + hits(one,three) + hits(one, four)
				+ hits(two,three) + hits(two, four)
				+ hits(three, four);
		},
		[&](const board &bo) -> const std::list<board> {
			std::list<board> ret;
			for (auto &q : queen_l){
				{
					board b = bo;
					b[q].first += 1;
					ret.push_back(b);
				}
				{
					board b = bo;
					b[q].first -= 1;
					ret.push_back(b);
				}
				{
					board b = bo;
					b[q].second += 1;
					ret.push_back(b);
				}
				{
					board b = bo;
					b[q].second -= 1;
					ret.push_back(b);
				}
			}
			return ret;
		});

	for (auto &p : final){
		std::cout << p.second.first << "," << p.second.second << std::endl;
	}
}

int main(){
	Grph g;
	auto n = Node::build(g,{});
	n->insert(*n);
	std::vector<Node_p > v;
	v.push_back(n);
	hillclimb(n, [](const decltype(n) &n){return n->neighbors.size();},
			  [](const auto &a){return strengthen<std::list<decltype(n)> >(a->neighbors);});
	queens();
}

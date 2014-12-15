#include <list>
#include <memory>
#include <functional>
#include "path.hpp"

template<typename Vec, typename Node, typename L>
auto minimax(const Node& a,
			 const std::function<int (decltype(a)) > &utility,
			 const std::function<bool (decltype(a), const Vec&, const Vec& )> &compare,
			 const L &children_f){
	const std::function<decltype(children_f(a)) (decltype(a)) > &children = children_f;

	Vec wantmax;
	path<Node> statemax(a,path<Node>::empty::EMPTY);
	const auto &kids = children(a);
	if (kids.size() == 0){
		return std::make_pair(utility(a),statemax);
	}
	else {
		for (const auto &c : kids){
			auto r = minimax(c, utility, compare, children);
			if (compare(a,r.first,wantmax) ) {
				wantmax = r.first;
				statemax = path<Node>(c, r.second);
			}
		}
		return std::make_pair(wantmax, statemax);
	}
}

int main(){
	
	minimax<int>(0, [](const int&){return 0;},
			[](const int&, const int &f, const int& o){return f < o; },
			[](const int&){return std::list<int>(); });
}

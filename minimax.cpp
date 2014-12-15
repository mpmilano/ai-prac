#include <list>
#include <memory>
#include <functional>
#include "path.hpp"

template<typename Node, typename L>
auto minimax(const Node& a,
			 const std::function<int (decltype(a)) > &utility,
			 const std::function<bool (decltype(a) )> &myturn,
			 const L &children_f){
	const std::function<decltype(children_f(a)) (decltype(a)) > &children = children_f;

	int wantmax = (myturn(a) ? 0 : std::numeric_limits<int>::max());
	path<Node> statemax(a,path<Node>::empty::EMPTY);
	const auto &kids = children(a);
	if (kids.size() == 0){
		return std::make_pair(utility(a),statemax);
	}
	else {
		for (const auto &c : kids){
			auto r = minimax(c, utility, myturn, children);
			if (myturn(a) ? r.first > wantmax : r.first < wantmax) {
				wantmax = r.first;
				statemax = path<Node>(c, r.second);
			}
		}
		return std::make_pair(wantmax, statemax);
	}
}

int main(){
	
	minimax(0, [](const int&){return 0;},
			[](const int&){return true; },
			[](const int&){return std::list<int>(); });
}

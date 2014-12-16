#include <list>
#include <memory>
#include <functional>
#include "path.hpp"

//might want to consider memoizing this. 
template<typename F, typename G, typename Node, typename L, typename Vec>
std::pair<Vec, path<Node> > minimax(const Node& a, const F &utility_f, const G &compare_f, const L &children_f,
			 std::function<decltype(a) (decltype(a))> parent, Vec& alphabeta){
	//typdef decltype(utility(a)) Vec;
	const std::function<decltype(children_f(a)) (decltype(a)) > &children = children_f;
	const std::function<Vec (decltype(a)) > &utility = utility_f;
	const std::function<bool (decltype(a), const Vec&, const Vec& )> &compare = compare_f;
		
	using namespace std::placeholders;
	auto minimax_r = std::bind(minimax<decltype(utility), decltype(compare), Node, decltype(children), Vec>, _1, utility, compare, children, parent, _2);

	Vec wantmax;
	path<Node> statemax(a,path<Node>::empty::EMPTY);
	const auto &kids = children(a);
	if (kids.size() == 0){
		return std::make_pair(utility(a),statemax);
	}
	else {
		Vec oldab = alphabeta;
		for (const auto &c : kids){
			auto r = minimax_r(c, alphabeta);
			if (compare(a,r.first,wantmax) ) {
				alphabeta = oldab;
				wantmax = r.first;
				statemax = path<Node>(c, r.second);
				if (compare(parent(a), r.first, alphabeta)) {
					oldab = alphabeta;
					alphabeta = r.first;
				} else break;
			}
		}
		return std::make_pair(wantmax, statemax);
	}
}

int main(){

	static int alphabeta = 0;
	minimax(0, [](const int&){return 0;},
			[](const int&, const int &f, const int& o){return f < o; },
				 [](const int&){return std::list<int>(); }, [&](const int &) -> const int& {return alphabeta;}, alphabeta);
}

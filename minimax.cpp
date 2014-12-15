#include <list>
#include <memory>
#include <functional>


template<typename Node>
struct path {
	enum class empty { EMPTY};
	const Node* elem;
	std::shared_ptr<const path> r;
	path(const Node& elem, decltype(r) r)
		:elem(&elem),r(r){}
	
	path(const Node& elem, path r)
		:elem(&elem),r(new path(r)){}

	path(const Node& elem, empty)
		:elem(&elem){}

	path(const path& p)
		:elem(p.elem),r(p.r){}
	
	path(const std::shared_ptr<const path>& p)
		:elem(p->elem),r(p->r){}

	bool operator==(const path &p) const{
		return (elem == p.elem) && r == p.r;
	}

	path operator=(const path &p){
		elem = p.elem;
		r = p.r;
		return *this;
	}
};

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

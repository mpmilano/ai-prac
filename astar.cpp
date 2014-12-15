#include <boost/variant.hpp>
#include <queue>
#include "inline_variant.hpp"
#include "Node.hpp"
#include "Worklist.hpp"

struct path {
	enum class empty { EMPTY};
	const Node& elem;
	const boost::variant<empty, const std::shared_ptr<path> > r;
	double weight;
	path(const Node& elem, const boost::variant<empty, const std::shared_ptr<path> > r, std::function<double (const Node&)> cost)
		:elem(elem),r(r),weight(match(r,
									  [](empty) -> double {return 0;},
									  [](const std::shared_ptr<path> r) -> double {return r->weight;})
								+ cost(elem)){}

	bool operator<(const path &p) const {
		return weight < p.weight;
	}
	bool operator==(const path &p) const{
		return weight == p.weight;
	}

	path (const path& path) = delete;
	
};

#define extend(el,p) std::shared_ptr<path>(new path(el,p,h));

Node_cp get_head(const std::shared_ptr<path>& p) {
	return p->elem.self();
}
std::shared_ptr<path> a_star(const Node &first, const Node& goal,
							 std::function<double (const Node& )> h){
	auto open = build_pwl(get_head);
	auto init = extend(first,path::empty::EMPTY);
	open.push(init);
	
	while (!open.empty()){
		auto &cur = open.top();
		if (cur->elem == goal) return cur;
		else for (auto &n : cur->elem.neighbors){
				auto newpath = extend(*n.lock(),cur);
				open.push(newpath);
			}
	}
	assert(false && "failed!");
	return init;
}

auto a_star(const Node &first, const Node &goal,
			std::function<double (const Node&, const Node&)> h){
	return a_star(first,goal,[&](const Node& a){return h(a,goal);});
}




int main(){
	Grph g;
	auto n = Node::build(g,{});
	std::vector<Node_p > v;
	v.push_back(n);
}

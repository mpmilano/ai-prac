#include <boost/variant.hpp>
#include <queue>
#include "inline_variant.hpp"
#include "Node.hpp"
#include "Worklist.hpp"

struct path {
	enum class empty { EMPTY};
	const Node& elem;
	const boost::variant<empty, const std::shared_ptr<path> > r;
	const double weight;
	const double estimate;
	path(const Node& elem,
		 //previous
		 const boost::variant<empty, const std::shared_ptr<path> > r,
		 //actual cost of taking just this hop in the path;
		 double weight,
		 //cost of taking imaginary straight-shot from
		 //hop end to goal.
		 std::function<double (const Node&)> cost)
		:elem(elem),r(r),
		 weight(weight + match(r,
							   [](empty) -> double {return 0;},
							   [](const std::shared_ptr<path> r) -> double {return r->weight;}))
		,estimate(cost(elem)){}


	bool operator<(const path &p) const {
		return weight + estimate < p.weight + p.estimate;
	}
	bool operator==(const path &p) const{
		return (&elem == &p.elem) && r == p.r;
	}

	double priority(){ return weight + estimate; }

	path (const path& path) = delete;
	
};

//TODO: assuming all paths have equal costs for some reason.
#define extend(el,p) std::shared_ptr<path>(new path(el,p,1,h));

Node_cp get_head(const std::shared_ptr<path>& p) {
	return p->elem.self();
}

auto pair_with_cost(const std::shared_ptr<path> &p){
	return std::make_pair(p->priority(), &p);
}

const auto& depair(const std::pair<double, const std::shared_ptr<path>* > &p){
	return *p.second;
}

std::shared_ptr<path> a_star(const Node &first, const Node& goal,
							 std::function<double (const Node& )> h){
	auto open = build_pwl(get_head, pair_with_cost, depair);
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

	auto wl = build_pwl<int>();
}

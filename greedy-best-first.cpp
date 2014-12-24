#include "Node.hpp"

std::list<Node_cp> greedy_bestfirst(const Node& first, const Node& goal,
									std::function<double (const Node&, const Node&)> h){
	std::map<const Node *const, bool> seenm;
	
	typedef std::list<Node_cp> path;
	typedef std::unique_ptr<std::list<Node_cp> > path_p;
	auto sortf = [&](Node_wp& l, Node_wp& r){return h(*l.lock(),goal) > h(*r.lock(),goal);};
	
	std::function<std::pair<bool, path_p> (const Node& , const Node& , const path& ) > f;
	f =	[&] (auto& first, auto& goal, auto& curpath) {
		path_p nextpath(new path(curpath));
		nextpath->push_back(first.self().lock());
		if (first.neighbors.size() == 0)
			return std::make_pair(first == goal, std::move(nextpath) );
		
		decltype(first.neighbors) newN = first.neighbors;
		newN.sort(sortf);
		for (auto &n : newN) {
			auto res = f(*(n.lock()),goal,*nextpath);
			if (res.first) return res;
		}
		return std::make_pair(false, std::move(nextpath) );
	};
	path initialpath;
	auto retpath = *((f(first,goal,initialpath)).second);
	return retpath;
}

int main(){
	Grph g;
	auto n = Node::build(g,{});
	std::vector<Node_p > v;
	v.push_back(n);
}

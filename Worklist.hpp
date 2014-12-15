template<typename T, typename U = T, typename V = T>
class PriorityWorklist {
	std::set<U> seen;
	std::set<T> holding;
	std::priority_queue<V> todo;
	std::function<U (const T&) > track;
	std::function<V (const T&) > pair;
	std::function<const T& (const V&) > depair;
public:
	PriorityWorklist(decltype(track) track, decltype(pair) pair, decltype(depair) depair)
		:track(track),pair(pair),depair(depair){}

	void push(const T& e){
		if (seen.find(track(e)) == seen.end()) {
			holding.insert(e);
			todo.push(pair(*holding.find(e)));
			seen.insert(track(e));
		}
	}

	const T& top(){
		const auto &ret = depair(todo.top());
		todo.pop();
		holding.erase(ret);
		return ret;
	}

	bool empty() {return todo.empty(); }

};

template<typename T, typename U, typename V>
auto build_pwl(const std::function<U (const T&) > &track,
			   const std::function<V (const T&) > &pair,
			   const std::function<const T& (const V&) > &depair){
	return PriorityWorklist<T,U,V>(track,pair,depair);
}

template<typename T, typename U, typename V>
auto build_pwl(U (*track) (const T&),
			   V (*pair) (const T&),
			   const T& (*depair) (const V&) ){
	PriorityWorklist<T,U,V> ret(track,pair,depair);
	return ret;
}

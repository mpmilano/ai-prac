template<typename T, typename U = T>
class PriorityWorklist {
	std::set<U> seen;
	std::priority_queue<T> todo;
	std::function<U (const T&) > track;
public:
	PriorityWorklist(decltype(track) track):track(track){}

	void push(const T& e){
		if (seen.find(track(e)) == seen.end()) {
			todo.push(e);
			seen.insert(track(e));
		}
	}

	const T& top(){
		const auto &ret = todo.top();
		todo.pop();
		return ret;
	}

	bool empty() {return todo.empty(); }

};

template<typename T, typename U>
PriorityWorklist<T,U> build_pwl(const std::function<U (const T&) > &track){
	return PriorityWorklist<T,U>(track);
}

template<typename T, typename U>
PriorityWorklist<T,U> build_pwl(U (*track) (const T&)){
	return PriorityWorklist<T,U>(track);
}

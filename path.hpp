#pragma once

//actually just a basic cons list.

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

	const Node& first() const {
		return *elem;
	}

	const auto* rest() const { return r.get(); }

};

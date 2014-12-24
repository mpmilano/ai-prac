#pragma once
#include <list>
#include <cassert>

namespace Tree{

	static constexpr short maxdepth = 51;
	
	template<typename E, typename TL, decltype(maxdepth) n = maxdepth>
	struct t{

		typedef t<E,TL,n-1> ctype;
		std::list<std::pair<TL, ctype > > children;
		E val;

		operator E&() {
			return val;
		}
		
		const operator E&() const {
			return val;
		}
		
	};
	
	template<typename E, typename TL>
	struct t<E, TL, 0>{
		E val;
	};

	template<typename E, typename TL, decltype(maxdepth) n>
	auto* children(t<E,TL,n> &t) {
		return &t.children;
	}

	template<typename E, typename TL>
	auto* children(t<E,TL,0> &) {
		assert(false && "max depth exceeded!");
		return nullptr;
	}

	template<typename E, typename TL, typename CV, typename CV2>
	auto& addChild(t<E,TL,0> &t, CV , CV2 &&){
		assert(false && "max depth exceeded!");
		return t;
	}
	
	template<typename E, typename TL, typename CV, typename CV2, decltype(maxdepth) n>
	auto& addChild(t<E,TL,n> &t, CV tl, CV2 &&e){
		typename std::decay<decltype(t.children.front().second)>::type nt;
		nt.val = e;
		t.children.push_back(std::make_pair(tl,nt));
		return t;
	}
	
	template<typename E, typename TL, typename CV, decltype(maxdepth) n>
	auto dangle(t<E,TL,n> &t, CV tl){
		return std::make_pair(tl,&t.children);
	}

	template<typename E, typename TL, typename CV>
	auto dangle(t<E,TL,0> &, CV tl){
		assert(false && "max depth exceeded!");
		std::list< std::pair<TL, t<E,TL,1> > > dummy;
		return std::make_pair(tl,&dummy);
	}

	template<typename T, typename E>
	auto& hookin(T c, E &&e){
		typename std::decay<decltype(c.second->front().second)>::type nt;
		nt.val = e;
		c.second->push_back(std::make_pair(c.first,nt));
		return c.second->back().second;
	}

}

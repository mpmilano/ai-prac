#pragma once
#include <memory>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <set>

class Node;

typedef std::shared_ptr<Node> Node_p;
typedef std::weak_ptr<Node> Node_wp;
typedef const std::shared_ptr<Node> Node_cp;

template<typename T>
bool operator==(const std::weak_ptr<T> &a, const std::weak_ptr<T> &b){
	return a.lock() == b.lock();
}

template<typename Node_p>
class Graph{
	std::set<Node_p> nodes;
public:
	void insert(Node_p at, Node_p in){
		nodes.insert(at);
		nodes.insert(in);
		at->neighbors.push_back(in);
	}
	friend class Node;
};

typedef Graph<Node_p> Grph;

class Node{
	Node_wp selfp;

	//template<class Collection>
	//Node(const Collection &c):neighbors(c.begin(),c.end()){}
	Node(Graph<Node_p> &g, std::initializer_list<Node_p > c):g(g){
		for (auto &n : c) neighbors.push_back(n);
	}
	
public:

	Graph<Node_p> &g;

	Node(const Node&) = delete;
	
	const Node_cp self() const {return selfp.lock();}
	std::list<Node_wp > neighbors;

	/*
	template<class Collection>
	static Node_p build(const Collection &c){
		Node_p ret(new Node(c));
		ret->selfp=ret;
		return ret;
		}//*/
	
	static Node_p build(Graph<Node_p> &g, std::initializer_list<Node_p > c){
		Node_p ret(new Node(g,c));
		ret->selfp=ret;
		g.nodes.insert(ret);
		return ret;
	}

	Node& insert(const Node &n){
		g.nodes.insert(n.self());
		neighbors.push_back(n.self());
		return *this;
	}

	bool operator== (const Node& n) const {
		return this == &n;
	}

	void remove(){
		for (auto &n : neighbors){
			auto o = std::find(n.lock()->neighbors.begin(), n.lock()->neighbors.end(),selfp);
			assert(o != n.lock()->neighbors.end());
			g.nodes.erase(o->lock());
			o->reset();
		}
	}
	friend class Graph<Node_p>;
};


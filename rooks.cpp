#include "genetic.hpp"
//8 queens on an 8x8 board

namespace rooks{
	constexpr int size = 8;
	typedef int rook;
	typedef std::array<rook,size> board;

	double rank (const board &b){
		double score = 0;
		std::array<bool, size> hits;
		for (int i  = 0; i < size; ++i) hits[i] = false;
		for (auto &col : b){
			if (hits[col]) ++score;
			else hits[col] = true;
		}

		return size - score;
	}

	void print(const board &b){
		std::cout << "<";
		for (auto &e : b) std::cout << e << ",";
		std::cout << ">" << std::endl;
	}

	bool done(const board &b){
		return rank(b) == size;
	}

	rook mutate(const rook &r){
		if (rand() % 12 == 3) return (r + 1) % size;
		else return r;
	}

	board generate(){
		std::array<rook,size> ret;
		for (int i = 0; i < size; ++i)
			ret[i] = rand() % size;
		return ret;
	}
}

int main(){

	srand(12);

	//generate, k, rank, done, mutate
	auto result = genetic(rooks::generate, 8, rooks::rank, rooks::done, rooks::mutate);
	rooks::print(result);
		
}

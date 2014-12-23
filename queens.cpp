#include <algorithm>
#include "genetic.hpp"
//8 queens on an 8x8 board

namespace queens{
	constexpr int size = 8;
	typedef int queen;
	typedef std::array<queen,size> board;

	double rank (const board &b){
		double score = 0;
		std::array<bool, size> hits;
		for (int i  = 0; i < size; ++i) hits[i] = false;

		//row intersection
		for (auto &col : b){
			if (hits[col]) ++score;
			else hits[col] = true;
		}

		//positive-slope diagonal intersection
		{
			int y = 0;
			for (auto it = b.begin(); it != b.end(); ++it, ++y){
				auto x = *it;
				for (int offs = std::max( -1 * x, -1 * y); ; ++offs){
					if (x + offs == (int) b.size()) break;
					assert(x + offs >= 0);
					if (b[x+offs] == y + offs) ++score;
				}
				
			}
		}


		//negative-slope diagonal intersection
		{
			int y = 0;
			for (auto it = b.begin(); it != b.end(); ++it, ++y){
				auto x = *it;
				for (int offs = std::max( -1 * x, y - size); ; ++offs){
					if (x + offs == (int) b.size()) break;
					assert(x + offs >= 0);
					if (b[x+offs] == y - offs) ++score;
				}
				
			}
		}

		return  -1 * score;
	}

	void print(const board &b){
		std::cout << "<";
		for (auto &e : b) std::cout << e << ",";
		std::cout << ">" << std::endl;
	}

	bool done(const board &b){
		return ((int) rank(b)) == 0;
	}

	queen mutate(const queen &r){
		if (rand() % 12 == 3) return (r + 1) % size;
		else return r;
	}

	board generate(){
		std::array<queen,size> ret;
		for (int i = 0; i < size; ++i)
			ret[i] = rand() % size;
		return ret;
	}
}

int main(){

	srand(12);

	//generate, k, rank, done, mutate
	auto result = genetic(queens::generate, 8, queens::rank, queens::done, queens::mutate);
	queens::print(result);
		
}

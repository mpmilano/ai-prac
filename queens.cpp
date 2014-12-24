#include <algorithm>
#include "genetic.hpp"
#include <time.h>
//8 queens on an 8x8 board

using namespace std;

namespace queens{
	constexpr int size = 8;
	typedef int queen;
	typedef std::array<queen,size> board;

	double rank (const board &b, bool flat = false){
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
			int x = 0;
			for (auto it = b.begin(); it != b.end(); ++it, ++x){
				auto y = *it;
				
				//positive-slope diagonal intersection
				for (int offs = std::max( -1 * x, -1 * y); y + offs < size; ++offs){
					if (x + offs == (int) b.size()) break;
					if (offs == 0) continue;
					assert(x + offs >= 0 && x + offs < size);
					assert(y + offs >= 0 && y + offs < size);
					if (b[x+offs] == y + offs) {
						//std::cout << "<" << x+ offs << "," << y + offs << "> intersects <" << x << "," << y << ">" << std::endl;
						++score;
					}
				}

				//negative-slope diagonal intersection
				for (int offs = std::max( -1 * x, y - (size - 1) ); y - offs >= 0; ++offs){
					assert(std::max( -1 * x, y - (size - 1) ) <= 0);
					if (x + offs == (int) b.size()) break;
					if (offs == 0) continue;
					assert(x + offs >= 0 && x + offs < size);
					assert(y - offs >= 0 && y - offs < size);
					if (b[x+offs] == y - offs) {
						++score;
					}
				}
			}
		}
		return (flat ?
				( score > 0 ? 1 : 0) :
				((size * size * size) - score));
	}

	void print(const board &b){
		std::cout << "<";
		for (auto &e : b) std::cout << e << ",";
		std::cout << ">" << std::endl;
	}

	bool done(const board &b){
		return ((int) rank(b)) == size * size * size;
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

	srand(time(NULL));

	clock_t start, end;
	double cpu_time_used;
	
	queens::board knowngood;
	knowngood[0] = 0;
	knowngood[1] = 4;
	knowngood[2] = 7;
	knowngood[3] = 5;
	knowngood[4] = 2;
	knowngood[5] = 6;
	knowngood[6] = 1;
	knowngood[7] = 3;

	if (!queens::done(knowngood)){
		std::cerr << queens::rank(knowngood) << std::endl;
		assert(queens::done(knowngood));
	}

	using namespace placeholders;
	auto rank1 = bind(queens::rank,_1, true);
	auto rank2 = bind(queens::rank,_1, false);


	for (int cnt = 0; cnt < 30; ++cnt){
		auto a = time(NULL);
	//generate, k, rank, done, mutate
	{

		srand(a);
		start = clock();
		auto result = genetic(queens::generate, 8, rank1, queens::done, queens::mutate);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		std::cout << "time for flat search: " << cpu_time_used << std::endl;
		queens::print(result);
	}
	
	{
		srand(a);
		start = clock();
		auto result = genetic(queens::generate, 8, rank2, queens::done, queens::mutate);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		std::cout << "time for guided search: " << cpu_time_used << std::endl;
		queens::print(result);
	}
	}
}

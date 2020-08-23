#include "ShantenCalculator.h"
#include <iostream>
#include <future>
#include <random>

int ShantenCalculator::CalculateShanten(std::vector<int>& hand)
{
	int* hand_ptr = &hand[0];
	int num_tiles = std::accumulate(hand.begin(), hand.end(), 0);
	int num_groups = (num_tiles - 2) / 3;
	int mode;
	return calsht(hand_ptr, num_groups, mode);
}

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::CalculateTwoStep(std::vector<int> hand, std::vector<int> wall)
{
	std::map<int, ImprovementCount> shanten_map;
	auto wallPairs = WallPairs(wall);

	int originalShanten = CalculateShanten(hand);

	for (auto elem : wallPairs)
	{
		int firstDraw = elem.first.first;
		int secondDraw = elem.first.second;
		int weight = elem.second;
		for (size_t i = 0; i < hand.size(); ++i) {
			if (hand[i] == 0) {
				continue;
			}
			int bestShantenDiff = -1;
			// Try discarding this tile for the first tile
			SwapTile(hand, i, firstDraw);
			for (size_t j = 0; j < hand.size(); ++j) {
				if (hand[j] == 0) {
					continue;
				}
				// Discard tile j for second tile
				SwapTile(hand, j, secondDraw);
				int shanten = CalculateShanten(hand);
				int shantenDiff = originalShanten - shanten;
				if (shantenDiff > bestShantenDiff) {
					bestShantenDiff = shantenDiff;
				}
				RevertSwap(hand, j, secondDraw);
			}
			RevertSwap(hand, i, firstDraw);
			auto& counter = shanten_map[i];
			if (bestShantenDiff < 0) {
				counter.Neg += weight;
			}
			else if (bestShantenDiff == 0) {
				counter.Zero += weight;
			}
			else if (bestShantenDiff == 1) {
				counter.One += weight;
			}
			else if (bestShantenDiff == 2) {
				counter.Two += weight;
			}
		}
	}

	return shanten_map;
}

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::CalculateThreeStep(std::vector<int> hand, std::vector<int> wall)
{
	std::map<int, ImprovementCount> shanten_map;

	auto wallTriples = WallTriples(wall);

	int originalShanten = CalculateShanten(hand);
	int iters = 0;
	for (auto elem : wallTriples)
	{
		/*
		if (iters % 100 == 0) {
			std::cout << "progress(" << iters << "/" << wallTriples.size() << ")" << std::endl;
		}
		*/
		const auto& tuple = elem.first;
		int firstDraw = std::get<0>(tuple);
		int secondDraw = std::get<1>(tuple);
		int thirdDraw = std::get<2>(tuple);
		int weight = elem.second;
		for (size_t i = 0; i < hand.size(); ++i) {
			if (hand[i] == 0) {
				continue;
			}
			int bestShantenDiff = -1;
			// Try discarding this tile for the first tile
			SwapTile(hand, i, firstDraw);
			for (size_t j = 0; j < hand.size(); ++j) {
				if (hand[j] == 0) {
					continue;
				}
				SwapTile(hand, j, secondDraw);
				for (size_t k = 0; k < hand.size(); ++k) {
					if ( hand[k] == 0) {
						continue;
					}
					SwapTile(hand, k, thirdDraw);
					int shanten = CalculateShanten(hand);
					int shantenDiff = originalShanten - shanten;
					if (shantenDiff > bestShantenDiff) {
						bestShantenDiff = shantenDiff;
					}
					RevertSwap(hand, k, thirdDraw);
				}
				RevertSwap(hand, j, secondDraw);
			}
			RevertSwap(hand, i, firstDraw);
			auto& counter = shanten_map[i];
			if (bestShantenDiff < 0) {
				counter.Neg += weight;
			}
			else if (bestShantenDiff == 0) {
				counter.Zero += weight;
			}
			else if (bestShantenDiff == 1) {
				counter.One += weight;
			}
			else if (bestShantenDiff == 2) {
				counter.Two += weight;
			}
			else if (bestShantenDiff == 3) {
				counter.Three += weight;
			}
		}
		iters += 1;
	}

	return shanten_map;
}

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::ThreeStepOnlyWallSection(
	std::vector<int> hand,
	const int originalShanten,
	std::vector<std::pair<std::tuple<int, int, int>, int>>::iterator begin,
	std::vector<std::pair<std::tuple<int, int, int>, int>>::iterator end
) {
	std::map<int, ImprovementCount> shanten_map;
	int iters = 0;
	for (;begin != end; ++begin)
	{
		auto& elem = *begin;
		const auto& tuple = elem.first;
		int firstDraw = std::get<0>(tuple);
		int secondDraw = std::get<1>(tuple);
		int thirdDraw = std::get<2>(tuple);
		int weight = elem.second;
		for (size_t i = 0; i < hand.size(); ++i) {
			if (hand[i] == 0) {
				continue;
			}
			int bestShantenDiff = -1;
			// Try discarding this tile for the first tile
			SwapTile(hand, i, firstDraw);
			if (originalShanten - CalculateShanten(hand) < 1) {
				RevertSwap(hand, i, firstDraw);
				continue;
			}
			for (size_t j = 0; j < hand.size(); ++j) {
				if (hand[j] == 0) {
					continue;
				}
				SwapTile(hand, j, secondDraw);
				if (originalShanten - CalculateShanten(hand) < 2) {
					RevertSwap(hand, j, secondDraw);
					continue;
				}
				for (size_t k = 0; k < hand.size(); ++k) {
					if (hand[k] == 0) {
						continue;
					}
					SwapTile(hand, k, thirdDraw);
					int shanten = CalculateShanten(hand);
					int shantenDiff = originalShanten - shanten;
					if (shantenDiff > bestShantenDiff) {
						bestShantenDiff = shantenDiff;
					}
					RevertSwap(hand, k, thirdDraw);
				}
				RevertSwap(hand, j, secondDraw);
			}
			RevertSwap(hand, i, firstDraw);
			auto& counter = shanten_map[i];
			if (bestShantenDiff < 0) {
				counter.Neg += weight;
			}
			else if (bestShantenDiff == 0) {
				counter.Zero += weight;
			}
			else if (bestShantenDiff == 1) {
				counter.One += weight;
			}
			else if (bestShantenDiff == 2) {
				counter.Two += weight;
			}
			else if (bestShantenDiff == 3) {
				counter.Three += weight;
			}
		}
		iters += 1;
	}
	return shanten_map;

}

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::CalculateThreeStepOnlyThreaded(std::vector<int> hand, std::vector<int> wall, int num_threads) {
	auto wallTriples = WallTriples(wall);
	int originalShanten = CalculateShanten(hand);

	// Shuffle triples for more uniform work distribution across threads.
	auto rng = std::default_random_engine{ std::random_device{}() }; // Create PRNG from seed.
	std::shuffle(std::begin(wallTriples), std::end(wallTriples), rng);

	int base_chunk_size = wallTriples.size() / num_threads;
	int rem = wallTriples.size() % num_threads;
	std::vector<std::future<std::map<int, ShantenCalculator::ImprovementCount>>> results;
	auto cur = wallTriples.begin();
	for (int thread_no = 0; thread_no < num_threads; ++thread_no) {
		int chunk_size = base_chunk_size;
		// Add remainder parts.
		if (thread_no < rem) {
			chunk_size += 1;
		}

		auto chunk_end = cur + chunk_size;
		results.push_back(std::async(
			std::launch::async,
			&ShantenCalculator::ThreeStepOnlyWallSection,
			this,
			hand,
			originalShanten,
			cur,
			chunk_end
		));

		cur = chunk_end;
	}

	std::map<int, ShantenCalculator::ImprovementCount> combined_shanten_map;
	// TODO: Could use accumulate instead
	for (auto& result : results) {
		auto shanten_map = result.get();
		for (auto& tile : shanten_map) {
			// Tile -> ImprovementCount
			combined_shanten_map[tile.first] += tile.second;
		}
	}

	return combined_shanten_map;
}

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::CalculateThreeStepOnly(std::vector<int> hand, std::vector<int> wall)
{
	auto wallTriples = WallTriples(wall);
	int originalShanten = CalculateShanten(hand);
	return ThreeStepOnlyWallSection(hand, originalShanten, wallTriples.begin(), wallTriples.end());
}


std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::GetOneShantenCounts(std::vector<int> hand, std::vector<int> wall)
{
	std::map<int, ImprovementCount> shanten_map;

	int originalShanten = CalculateShanten(hand);
	for (int draw = 0; draw < wall.size(); ++draw) {
		int weight = wall[draw];
		if (weight == 0) {
			continue;
		}
		for (int i = 0; i < hand.size(); ++i) {
			if (hand[i] == 0) {
				continue;
			}
			SwapTile(hand, i, draw);
			int shanten = CalculateShanten(hand);
			int shantenDiff = originalShanten - shanten;
			auto& counter = shanten_map[i];
			if (shantenDiff < 0) {
				counter.Neg += weight;
			}
			else if (shantenDiff == 0) {
				counter.Zero += weight;
			}
			else if (shantenDiff == 1) {
				counter.One += weight;
			}
			else if (shantenDiff == 2) {
				counter.Two += weight;
			}
			RevertSwap(hand, i, draw);
		}
	}

	return shanten_map;
}

int ShantenCalculator::GetShanten(std::vector<int> hand)
{
	return CalculateShanten(hand);
}

// Assumes wall is a 34-vector.
std::map<std::pair<int, int>, int> ShantenCalculator::WallPairs(std::vector<int> wall)
{
	std::map<std::pair<int, int>, int> pairs;
	for (size_t i = 0; i < wall.size(); ++i) {
		for (size_t j = 0; j < wall.size(); ++j) {
			int weight;
			if (i != j) {
				weight = wall[i] * wall[j];
			}
			else {
				weight = wall[i] * (wall[i] - 1);
			}
			if (weight != 0) {
				pairs.insert({ {i, j}, weight });
			}
		}
	}
	return pairs;
}

std::vector < std::pair< std::tuple<int, int, int>, int > > ShantenCalculator::WallTriples(std::vector<int> wall)
{
	std::vector < std::pair< std::tuple<int, int, int>, int > >  triples;
	for (size_t i = 0; i < wall.size(); ++i) {
		for (size_t j = 0; j < wall.size(); ++j) {
			for (size_t k = 0; k < wall.size(); k++) {
				int weight;
				int a = wall[i];
				int b = wall[j];
				int c = wall[k];
				if (a == 0 || b == 0 || c == 0) {
					continue;
				}
				if (i == j && j == k) {
					weight = a * (a - 1) * (a - 2);
				}
				else if (i == j) {
					weight = a * (a - 1) * c;
				}
				else if (i == k) {
					weight = a * b * (a - 1);
				}
				else if (j == k) {
					weight = a * b * (b - 1);
				}
				else {
					weight = a * b * c;
				}
				if (weight != 0) {
					triples.push_back({ {i, j, k}, weight });
				}
			}
		}
	}

	return triples;
}


void ShantenCalculator::SwapTile(std::vector<int>& hand, int tile1, int tile2)
{
	hand[tile1] -= 1;
	hand[tile2] += 1;
}

void ShantenCalculator::RevertSwap(std::vector<int>& hand, int tile1, int tile2)
{
	hand[tile1] += 1;
	hand[tile2] -= 1;
}

#include "ShantenCalculator.h"
#include <iostream>
#include <future>
#include <random>
#include <algorithm>

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

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::CalculateThreeStepRecur(std::vector<int> hand, std::vector<int> wall) {
	return CalculateStepRecur(hand, wall, 3);
}

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::CalculateStepRecur(std::vector<int> hand, std::vector<int> wall, int steps) {
	int original_shanten = CalculateShanten(hand);
	std::map<int, ShantenCalculator::ImprovementCount> combined_shanten_map;
	for (int tile_type = 0; tile_type < hand.size(); ++tile_type) {
		int tile_count = hand[tile_type];
		if (tile_count == 0) {
			continue;
		}

		std::vector<int> discarded(hand);
		--discarded[tile_type];

		int paths = 1;
		int cur_depth = 0;
		int num_paths = RecurShanten(discarded, wall, original_shanten, cur_depth, paths, steps - 1); // steps are zero indexed
		combined_shanten_map[tile_type] = { 0, 0, 0, 0, num_paths };
	}

	return combined_shanten_map;
}

int ShantenCalculator::RecurShanten(std::vector<int>& hand, std::vector<int>& wall, int cur_shanten, int cur_depth, int cur_paths, int max_depth) {
	int ret = 0;
	for (int wall_tile_type = 0; wall_tile_type < wall.size(); ++wall_tile_type) {
		int wall_tile_count = wall[wall_tile_type];
		if (wall_tile_count == 0) {
			continue;
		}
		int new_num_paths = cur_paths * wall_tile_count;

		// Add tile to hand.
		--wall[wall_tile_type];
		++hand[wall_tile_type];
		int new_shanten = CalculateShanten(hand);
		// Check for improvement.
		if (new_shanten < cur_shanten) {
			if (cur_depth == max_depth) {
				ret += new_num_paths;
			}
			else {
				int best_paths = 0;
				// Otherwise recurse.
				for (int hand_tile_type = 0; hand_tile_type < hand.size(); ++hand_tile_type) {
					int hand_tile_count = hand[hand_tile_type];
					if (hand_tile_count == 0) {
						continue;
					}
					--hand[hand_tile_type];
					best_paths = std::max(best_paths, RecurShanten(hand, wall, new_shanten, cur_depth + 1, new_num_paths, max_depth));

					++hand[hand_tile_type];
				}
				ret += best_paths;

			}


		}

		// Revert adding tile to hand.
		--hand[wall_tile_type];
		++wall[wall_tile_type];
	}
	return ret;
}

void ShantenCalculator::print_hand(std::vector<int> hand) {
	bool had_m = false;
	bool had_p = false;
	bool had_s = false;
	bool had_z = false;
	for (int i = 0; i < 9; i++) {
		if (hand[i] > 0) {
			had_m = true;
			for (int j = 0; j < hand[i]; ++j) {
				std::cout << (i % 9) + 1;
			}
		}
	}
	if (had_m) {
		std::cout << 'm';
	}
	for (int i = 9; i < 18; i++) {
		if (hand[i] > 0) {
			had_p = true;
			for (int j = 0; j < hand[i]; ++j) {
				std::cout << (i % 9) + 1;
			}
		}
	}
	if (had_p) {
		std::cout << 'p';
	}
	for (int i = 18; i < 27; i++) {
		if (hand[i] > 0) {
			had_s = true;
			for (int j = 0; j < hand[i]; ++j) {
				std::cout << (i % 9) + 1;
			}
		}
	}
	if (had_s) {
		std::cout << 's';
	}
	for (int i = 27; i < 34; i++) {
		if (hand[i] > 0) {
			had_z = true;
			for (int j = 0; j < hand[i]; ++j) {
				std::cout << (i % 9) + 1;
			}
		}
	}
	if (had_z) {
		std::cout << 'z';
	}


	std::cout << std::endl;
}

void ShantenCalculator::print_tile(int n) {
	std::cout << (n % 9) + 1;
	if (n < 9) {
		std::cout << 'm';
	} else if (n < 18) {
		std::cout << 'p';
	} else if (n < 27) {
		std::cout << 's';
	} else {
		std::cout << 'z';
	}
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

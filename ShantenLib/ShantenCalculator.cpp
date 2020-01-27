#include "ShantenCalculator.h"
#include <iostream>

int ShantenCalculator::CalculateShanten(std::vector<int>& hand)
{
	int* hand_ptr = &hand[0];
	int num_tiles = std::accumulate(hand.begin(), hand.end(), 0);
	int num_groups = (num_tiles - 2) / 3;
	int mode;
	return calsht(hand_ptr, num_groups, mode);
}

std::map<std::pair<int, int>, ShantenCalculator::ImprovementCount> ShantenCalculator::CalculateTwoStep(std::vector<int> hand, std::vector<int> wall)
{
	std::map<std::pair<int, int>, ImprovementCount> shanten_map;
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
				if (shanten_map.find({ i, j }) == shanten_map.end()) {
					shanten_map.insert({ { i, j }, ImprovementCount() });
				}
				auto& counter = shanten_map[{i, j}];
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
				RevertSwap(hand, j, secondDraw);
			}
			RevertSwap(hand, i, firstDraw);
		}
	}

	return shanten_map;
}

std::map<std::pair<int, int>, ShantenCalculator::ImprovementCountList> ShantenCalculator::CalculateTwoStepList(std::vector<int> hand, std::vector<int> wall)
{
	std::map<std::pair<int, int>, ImprovementCountList> shanten_map;
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
				if (shanten_map.find({ i, j }) == shanten_map.end()) {
					shanten_map.insert({ { i, j }, ImprovementCountList() });
				}
				auto& counter = shanten_map[{i, j}];
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
					counter.TwoImprovers.push_back({ firstDraw, secondDraw, weight });
					counter.Two += weight;
				}
				RevertSwap(hand, j, secondDraw);
			}
			RevertSwap(hand, i, firstDraw);
		}
	}

	return shanten_map;
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
			if (shanten_map.find(i) == shanten_map.end()) {
				shanten_map.insert({ i, ImprovementCount() });
			}
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

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::AccumulateChoices(std::map<std::pair<int, int>, ImprovementCount> twoStepMap)
{
	std::map<int, ShantenCalculator::ImprovementCount> acc;
	for (auto elem : twoStepMap)
	{
		auto& discard = elem.first.first;
		auto& counter = elem.second;
		if (acc.find(discard) == acc.end()) {
			acc.insert({ discard, ImprovementCount() });
		}
		auto& accCounter = acc[discard];
		accCounter.Neg += counter.Neg;
		accCounter.Zero += counter.Zero;
		accCounter.One += counter.One;
		accCounter.Two += counter.Two;

	}
	return acc;

}

std::map<int, ShantenCalculator::ImprovementCount> ShantenCalculator::GetTwoShantenCounts(std::vector<int> hand, std::vector<int> wall)
{
	return AccumulateChoices(CalculateTwoStep(hand, wall));
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

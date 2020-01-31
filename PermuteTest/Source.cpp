#include<iostream>
#include<string>
#include<map>
#include<vector>

using namespace std;

std::map<std::tuple<int, int, int>, int> WallTriples(std::vector<int> wall)
{
	std::map<std::tuple<int, int, int>, int> triples;
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
					triples.insert({ {i, j, k}, weight });
				}
			}
		}
	}

	return triples;
}

int main() {
	std::vector<int> a{ 4,4,4,4,4,4,4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4, 4,4,4,4,4,4,4, 4,4,4,4,4,4};
	auto v = WallTriples(a);
	std::cout << "len: " << v.size() << std::endl;
	int sum = 0;
	for (const auto& t : v) {
		auto weight = t.second;
		auto card = t.first;
		//std::cout << std::get<0>(card) << " " << std::get<1>(card) << " " << std::get<2>(card) << " " << weight << std::endl;
		sum += weight;
	}
	std::cout << "sum: " << sum << std::endl;
    return 0;
}
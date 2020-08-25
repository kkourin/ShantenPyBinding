#pragma once
#include "calsht.h"
#include <map>
#include <vector>
#include <tuple>

class ShantenCalculator {

    public:
        struct ImprovementCount {
            int Neg = 0;
            int Zero = 0;
            int One = 0;
            int Two = 0;
            int Three = 0;

            ImprovementCount& operator+=(const ImprovementCount& other) {
                Neg += other.Neg;
                Zero += other.Zero;
                One += other.One;
                Two += other.Two;
                Three += other.Three;
                return *this;
            }
        };

        ShantenCalculator() = default;
        int CalculateShanten(std::vector<int>& hand);
        // Returns <(first draw, second draw), (best shanten, weight)>
        std::map<int, ImprovementCount> CalculateTwoStep(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> CalculateThreeStep(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> CalculateThreeStepOnly(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> CalculateThreeStepOnlyThreaded(std::vector<int> hand, std::vector<int> wall, int num_threads);
        std::map<int, ImprovementCount> CalculateThreeStepRecur(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> GetOneShantenCounts(std::vector<int> hand, std::vector<int> wall);
        int GetShanten(std::vector<int> hand);
        static std::map<std::pair<int, int>, int> WallPairs(std::vector<int> wall);
        static std::vector < std::pair< std::tuple<int, int, int>, int > > WallTriples(std::vector<int> wall);

    private:
        Calsht calsht;
        void SwapTile(std::vector<int>& hand, int tile1, int tile2);
        void RevertSwap(std::vector<int>& hand, int tile1, int tile2);
        std::map<int, ImprovementCount> CalculateStepRecur(std::vector<int> hand, std::vector<int> wall, int steps);
        int RecurShanten(std::vector<int>& hand, std::vector<int>& wall, int cur_shanten, int cur_depth, int cur_paths, int max_depth);
        std::map<int, ShantenCalculator::ImprovementCount> ThreeStepOnlyWallSection(
            std::vector<int> hand,
            const int originalShanten,
            std::vector<std::pair<std::tuple<int, int, int>, int>>::iterator begin,
            std::vector<std::pair<std::tuple<int, int, int>, int>>::iterator end
        );
        static void print_hand(std::vector<int> hand);
        static void print_tile(int n);

};
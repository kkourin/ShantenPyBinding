#pragma once
#include "calsht.h"
#include <map>
#include <vector>

class ShantenCalculator {

    public:
        struct ImprovementCount {
            int Neg = 0;
            int Zero = 0;
            int One = 0;
            int Two = 0;
        };
        ShantenCalculator() = default;
        int CalculateShanten(std::vector<int>& hand);
        // Returns <(first draw, second draw), (best shanten, weight)>
        std::map<std::pair<int, int>, ImprovementCount> CalculateTwoStep(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> GetOneShantenCounts(std::vector<int> hand, std::vector<int> wall);
        static std::map<std::pair<int, int>, int> WallPairs(std::vector<int> wall);
        static std::map<int, ImprovementCount> AccumulateChoices(std::map<std::pair<int, int>, ImprovementCount> twoStepMap);
        std::map<int, ImprovementCount> GetTwoShantenCounts(std::vector<int> hand, std::vector<int> wall);
    private:
        Calsht calsht;
        void SwapTile(std::vector<int>& hand, int tile1, int tile2);
        void RevertSwap(std::vector<int>& hand, int tile1, int tile2);


};
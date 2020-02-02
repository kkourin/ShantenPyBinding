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
        };

        ShantenCalculator() = default;
        int CalculateShanten(std::vector<int>& hand);
        // Returns <(first draw, second draw), (best shanten, weight)>
        std::map<int, ImprovementCount> CalculateTwoStep(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> CalculateThreeStep(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> CalculateThreeStepOnly(std::vector<int> hand, std::vector<int> wall);
        std::map<int, ImprovementCount> GetOneShantenCounts(std::vector<int> hand, std::vector<int> wall);
        int GetShanten(std::vector<int> hand);
        static std::map<std::pair<int, int>, int> WallPairs(std::vector<int> wall);
        static std::vector < std::pair< std::tuple<int, int, int>, int > > WallTriples(std::vector<int> wall);

    private:
        Calsht calsht;
        void SwapTile(std::vector<int>& hand, int tile1, int tile2);
        void RevertSwap(std::vector<int>& hand, int tile1, int tile2);


};
//
// Created by AyanMR on 25-6-27.
//

#include "random.h"

std::vector<int> genRanNum()
{
    std::vector res(54, 0);
    for (int i = 0 ; i < 54 ; ++i)
        res[i] = i + 1;
    std::random_device rd;
    std::mt19937       gen(rd());
    std::ranges::shuffle(res, gen);
    return res;
}

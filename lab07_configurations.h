#pragma once
#include <ga/ga.h>
#include <ga/std_stream.h>
#include <array>
#define desc(s,v) std::make_pair(s,v)

struct lab07_conf_all{
    const std::array<std::pair<std::string, GAGenome::SexualCrossover>, 9> crossover_conf = {{
        desc("GARealUniformCrossover",GARealUniformCrossover),
        desc("GARealEvenOddCrossover",GARealEvenOddCrossover),
        desc("GARealOnePointCrossover",GARealOnePointCrossover),
        desc("GARealTwoPointCrossover",GARealTwoPointCrossover),
        desc("GARealPartialMatchCrossover",GARealPartialMatchCrossover),
        desc("GARealOrderCrossover",GARealOrderCrossover),
        desc("GARealCycleCrossover",GARealCycleCrossover),
        desc("GARealArithmeticCrossover",GARealArithmeticCrossover),
        desc("GARealBlendCrossover",GARealBlendCrossover)
    }};

    const std::array<std::pair<std::string, GAGenome::Mutator>, 2>mutator_conf = {{
        desc("GARealSwapMutator",GARealSwapMutator),        
        desc("GARealUniformMutator",GARealUniformMutator)
    }};
    
};

struct lab07_conf{
    //numeric values
    float pCross, pMut;
    //
    std::pair<std::string, GAGenome::SexualCrossover> curr_cross;
    std::pair<std::string, GAGenome::Mutator> curr_mut;
};

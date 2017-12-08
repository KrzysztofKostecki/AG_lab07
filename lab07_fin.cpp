// lab07_AG.cpp : Defines the entry point for the console application.
//

#include <StaticLib.h>

#include <ga/ga.h>
#include <ga/std_stream.h>
#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>
#include <vector>

#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>
#include "lab07_configurations.h"

#define DIM 2 //number of dimensions
#define FUNC_NUM 3
#define SEARCH_MIN -100.0
#define SEARCH_MAX 100.0

using namespace std;


struct Results
{
    float online;
    float offline;
    float best;
    float aveLast; //average of last population
    Results():
    online(0.0),
    offline(0.0),
    best(0.0),
    aveLast(0.0)
    {}
};

float
Objective(GAGenome &);
// float Comparator(const GAGenome &g1, const GAGenome &g2);
void save_stats(const lab07_conf &conf, const Results& result);
Results run_prepaerd_ga(const lab07_conf &conf);
Results avg_results(std::vector<Results>& res);
std::vector<lab07_conf> generate_all_configurations(lab07_conf_all &conf);


int main(int argc, char **argv)
{
	std::cout << GA_Get_Min_x(FUNC_NUM) << std::endl;
	std::cout << GA_Get_Max_x(FUNC_NUM) << std::endl;
	std::cout << GA_Get_Min_y(FUNC_NUM) << std::endl;
	std::cout << GA_Get_Max_y(FUNC_NUM) << std::endl;
	return 0;
    lab07_conf_all conf;
	std::vector<lab07_conf> all_confs;
	/*all_confs.push_back(lab07_conf( GARealUniformCrossover, GARealSwapMutator, 0.3,0.9 ));
	all_confs.push_back(lab07_conf{ GARealUniformCrossover, GARealSwapMutator, 0.4,0.8 });
	all_confs.push_back(lab07_conf{ GARealOnePointCrossover, GARealSwapMutator, 0.3,0.9 });*/
    for (int i = 0; i < all_confs.size(); i++)
    {
        std::vector<Results> test;
        for(int j = 0; j < 1; j++)
            test.push_back(run_prepaerd_ga(all_confs[i]));
        
        save_stats(all_confs[i],avg_results(test));
    }

    return 0;
}

float Objective(GAGenome &g)
{
    GARealGenome &genome = (GARealGenome &)g;
    return std::pow(genome.gene(0), 2) + std::pow(genome.gene(1), 2);
}

Results run_prepaerd_ga(const lab07_conf &conf)
{
    Results r;
    GARealAlleleSet alleleSet(SEARCH_MIN, SEARCH_MAX);
    GARealGenome genome(DIM, alleleSet, Objective);

    genome.crossover(conf.curr_cross.second);
    genome.mutator(conf.curr_mut.second);
   // genome.initializer(conf.curr_init.second);
    GASimpleGA ga(genome);
    ga.minimize();
    ga.populationSize(50);
    ga.nGenerations(100);
    ga.pMutation(conf.pMut);
    ga.pCrossover(conf.pCross);

    ga.evolve();

    //and save crucial results
    r.best = ga.statistics().minEver();
    r.online = ga.statistics().online();
    r.offline = ga.statistics().offlineMin();
    r.aveLast = ga.statistics().current(GAStatistics::Mean);
    return r;
}

void save_stats(const lab07_conf &conf, const Results& result)
{
    static int test_number = 1;

    std::string cwd = "results/";
    ofstream outfile;
    std::ostringstream file_name;

    if (result.aveLast > 0.5)
        return;
    file_name << cwd << "test_" << std::setfill('0') << std::setw(4) << test_number++ << ".dat";
    outfile.open(file_name.str(), (ios::out | ios::trunc));
    //print info about test
    outfile << "#" << std::endl;
    outfile << "# Crossover: " << conf.curr_cross.first << std::endl;
    outfile << "# Mutator: " << conf.curr_mut.first << std::endl;
    outfile << "# pMutation: " << conf.pMut << std::endl;
    outfile << "# pCrossover: " << conf.pCross << std::endl;
    outfile << "#" << std::endl;
    outfile << "# Results" << std::endl;
    outfile << "# On-line: " << result.online << std::endl;
    outfile << "# Off-line: " << result.offline << std::endl;
    outfile << "# Best: " << result.best << std::endl;
    outfile << "# Avg of last population: " << result.aveLast << std::endl;
    outfile.close();
}

std::vector<lab07_conf> generate_all_configurations(lab07_conf_all &conf)
{
    std::vector<lab07_conf> toReturn;
    for (auto &cross : conf.crossover_conf)
        for (auto &mutate : conf.mutator_conf)
            {
                float mut_beg = 0.1;
                float cross_beg = 0.9;
                for (int mut_n = 0; mut_n < 5; mut_n++)
                    for (int cross_n = 0; cross_n < 5; cross_n++)
                    {
                        lab07_conf c;
                        c.pMut = mut_beg + static_cast<float>(mut_n / 10.0);
                        c.pCross = cross_beg  - static_cast<float>(cross_n / 10.0);
                        c.curr_cross = cross;
                        c.curr_mut = mutate;
                        toReturn.push_back(c);
                    }
            }
    return toReturn;
}  

Results avg_results(std::vector<Results>& res) {
	Results sum_res;
	for (int i = 0; i < res.size(); i++) {
		sum_res.online += res[i].online;
		sum_res.offline += res[i].offline;
		sum_res.best += res[i].best;
		sum_res.aveLast += res[i].aveLast;
	}
	//count avg
	sum_res.online /= res.size();
	sum_res.offline /= res.size();
	sum_res.best /= res.size();
	sum_res.aveLast /= res.size();

	return sum_res;
}
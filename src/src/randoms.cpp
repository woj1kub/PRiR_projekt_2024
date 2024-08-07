#include "../headers/global.h"
#include "../headers/randoms.h"
#include <random>
#include <ctime>


int generate_random_number(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    int random_number = distribution(generator);
    return random_number;
}

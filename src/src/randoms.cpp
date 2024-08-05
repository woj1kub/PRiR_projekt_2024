#include "../headers/randoms.h"
#include "../headers/global.h"
#include <random>
#include <ctime>

std::mt19937 generator(static_cast<unsigned int>(seed));

int generate_random_number(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    int random_number = distribution(generator);
    return random_number;
}

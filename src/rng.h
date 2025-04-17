#pragma once
#include <random>

class Rng {
private:
  std::mt19937 gen;

public:
  // use specified seed to make random numbers predictable
  inline Rng(int seed) : gen(seed) {}

  // use seed from hardware random number generator
  inline Rng() : gen(std::random_device{}()) {}

  // generate random integer from low to high inclusive.  
  // ex randint(1,6) will produce one of the following numbers: 1,2,3,4,5,6
  inline int randint(int low, int high) {
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
  }
};
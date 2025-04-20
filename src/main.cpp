#include <iostream>
#include <string>
#include "ktest.hpp"
#include "rng.h"
#include "foodstuff.h"
#include "heap.h"   // TODO
#include "functors.h" // TODO

KTEST(test) {
    std::cout << "doing test." << std::endl;
    const bool myTrue = true;
    KASSERT_TRUE(myTrue);
}

KTEST(test2) {
    std::cout << "doing other test." << std::endl;
    KASSERT_EQ(5, 2 + 3) << "Something is very wrong with math";
}

KTEST(test3) {
    int i = 0;
    KASSERT_THROWS(std::runtime_error, [&], {
        i = 2;
        throw std::runtime_error("UwU");
    }) << "here goes nothing!";
    std::cout << "i: " << i << std::endl;
}

void makeHotDogs() {
    Rng rng(21324);

    Heap<Foodstuff, Cheapest> h;
    std::vector<Foodstuff> ingredients;

    for(int i = 0; i < 10; i++) {
        while(h.size() < 10) {
            Foodstuff ingredient = getRandomFoodstuff(rng);
            h.add(ingredient);
        }
        ingredients.push_back(h.remove());
    }

    int totalCost = 0;
    int totalWeight = 0;
    for(Foodstuff ingredient : ingredients) {
        printf("%-12s - cost: %3d  weight: %3d  CostPerPound: %3f\n", ingredient.name.c_str(), ingredient.cost, ingredient.weight, ingredient.getCostPerPound());
        totalCost += ingredient.cost;
        totalWeight += ingredient.weight;
    }
    std::cout << "total cost:   " << totalCost << "\n";
    std::cout << "total weight: " << totalWeight << "\n";
}


int main() {
    ktest::runAllTests();
    makeHotDogs();
    return 0;
}

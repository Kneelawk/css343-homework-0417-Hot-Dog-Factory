#include <iostream>
#include <string>
#include "ktest.hpp"
#include "rng.h"
#include "foodstuff.h"
#include "heap.h"   // TODO
#include "functors.h" // TODO

KTEST(heap_constructor) {
    const Heap<size_t, SimpleHeapComparator<size_t> > h;

    KASSERT_EQ(0, h.size()) << "initial size";
}

KTEST(heap_add) {
    Heap<size_t, SimpleHeapComparator<size_t> > h;
    KASSERT_EQ(0, h.size());

    for (size_t i = 0; i < 10; ++i) {
        KASSERT_EQ(i, h.size()) << "add i: " << i;
        h.add(i);
    }
    KASSERT_EQ(10, h.size()) << "final add";
}

KTEST(heap_remove) {
    Heap<size_t, SimpleHeapComparator<size_t> > h;
    KASSERT_EQ(0, h.size()) << "initial size";

    for (size_t i = 0; i < 100; ++i) {
        KASSERT_EQ(i, h.size()) << "add i: " << i;
        h.add(i);
    }
    KASSERT_EQ(100, h.size()) << "final add";

    for (size_t i = 99; true; --i) {
        KASSERT_EQ(i, h.remove()) << "remove i: " << i;
        KASSERT_EQ(i, h.size()) << "remove size i: " << i;

        if (i == 0) {
            break;
        }
    }
}

KTEST(rng_heap_remove) {
    Rng rng(21324);

    Heap<size_t, SimpleHeapComparator<size_t> > h;
    KASSERT_EQ(0, h.size()) << "initial size";

    for (size_t i = 0; i < 100; ++i) {
        KASSERT_EQ(i, h.size()) << "add i: " << i;
        const int val = rng.randint(0, 100);
        h.add(val);
    }
    KASSERT_EQ(100, h.size()) << "final add";

    size_t prev = 1000;
    for (size_t i = 0; i < 100; ++i) {
        size_t removed = h.remove();
        if (prev != 1000) {
            KASSERT_GE(prev, removed) << "prev should be greater than current removed";
        }
        prev = removed;
        KASSERT_EQ(99 - i, h.size()) << "remove size i: " << i;
    }
}

KTEST(foodstuff_heap_remove) {
    Rng rng(21324);

    Heap<Foodstuff, Cheapest> h;
    KASSERT_EQ(0, h.size()) << "initial size";

    for (size_t i = 0; i < 100; i++) {
        KASSERT_EQ(i, h.size()) << "add i: " << i;
        const Foodstuff foodstuff = getRandomFoodstuff(rng);
        h.add(foodstuff);
    }
    KASSERT_EQ(100, h.size()) << "final add";

    double prev = NAN;
    for (size_t i = 0; i < 100; i++) {
        Foodstuff removed = h.remove();
        if (!std::isnan(prev)) {
            KASSERT_LE(prev, removed.getCostPerPound()) << "prev should be less than current removed";
        }
        prev = removed.getCostPerPound();
        KASSERT_EQ(99 - i, h.size()) << "remove size i: " << i;
    }
}

void makeHotDogs() {
    Rng rng(21324);

    Heap<Foodstuff, Cheapest> h;
    std::vector<Foodstuff> ingredients;

    for (int i = 0; i < 10; i++) {
        while (h.size() < 10) {
            Foodstuff ingredient = getRandomFoodstuff(rng);
            h.add(ingredient);
        }
        ingredients.push_back(h.remove());
    }

    int totalCost = 0;
    int totalWeight = 0;
    for (Foodstuff ingredient: ingredients) {
        printf("%-12s - cost: %3d  weight: %3d  CostPerPound: %3f\n", ingredient.name.c_str(), ingredient.cost,
               ingredient.weight, ingredient.getCostPerPound());
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

#pragma once
#include <string>
#include "rng.h"

using std::string;

struct Foodstuff {
    string name;
    int weight;
    int cost;

    Foodstuff(string name, int weight, int cost) 
        : name(name), weight(weight), cost(cost) {;}

    double getCostPerPound() const {
        return (double) cost / (double) weight;
    }
};


inline Foodstuff getRandomFoodstuff(Rng& rng) {
    const string names[] = {"offal", "bones", "viscera", "hair", "fruit peels",
                            "old tires", "coffee grounds", "corn cobs", "nut shells", "pomace" };
    int nameidx = rng.randint(0,sizeof(names)/sizeof(string)-1);                          
    int weight = rng.randint(10,100);
    int cost = rng.randint(10,100);

    return Foodstuff( names[nameidx],weight, cost);
}


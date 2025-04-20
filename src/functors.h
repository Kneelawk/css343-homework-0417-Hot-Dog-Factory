//
// Created by cyan on 4/19/25.
//

#ifndef FUNCTORS_H
#define FUNCTORS_H

class Cheapest {
public:
    bool operator()(const Foodstuff &a, const Foodstuff &b) const {
        return a.getCostPerPound() > b.getCostPerPound();
    }
};

#endif //FUNCTORS_H

//
// Created by cyan on 4/20/25.
//

#include "ktest.hpp"
#include "rng.h"
#include "foodstuff.h"
#include "heap.h"
#include "functors.h"

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

//
// Created by cyan on 4/19/25.
//

#ifndef HEAP_H
#define HEAP_H

template<typename T>
class SimpleHeapComparator {
public:
    bool operator()(const T &a, const T &b) const {
        return a < b;
    }
};

// Absolutely no STL usage!
template<typename T, typename C>
class Heap final {
    T **data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    C comparator_;

    template<typename E>
    static void copy(E *first, const E *last, E *result) {
        while (first < last) {
            *result++ = *first++;
        }
    }

    void ensureCapacity(const size_t atLeast) {
        if (capacity_ < atLeast) {
            size_t newCapacity = capacity_;
            if (newCapacity < 4) {
                newCapacity = 4;
            }
            while (newCapacity < atLeast) {
                newCapacity *= 2;
            }

            T **newData = new T*[newCapacity]();

            if (capacity_ > 0) {
                copy(data_, data_ + capacity_, newData);
                delete[] data_;
            }

            data_ = newData;
            capacity_ = newCapacity;
        }
    }

    void upheap(size_t i) {
        T *val = data_[i];
        while (i > 0 && comparator_(*data_[(i - 1) / 2], *val)) {
            data_[i] = data_[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        data_[i] = val;
    }

    void downheap(size_t i) {
        T *val = data_[i];
        while (i < size_ / 2) {
            size_t childI = i * 2 + 1;
            if (childI < size_ - 1 && comparator_(*data_[childI], *data_[childI + 1])) {
                // select the larger of the two children
                ++childI;
            }
            if (comparator_(*data_[childI], *val)) {
                // we found our spot
                break;
            }
            // move stuff up the heap
            data_[i] = data_[childI];
            i = childI;
        }
        data_[i] = val;
    }

public:
    ~Heap() {
        for (size_t i = 0; i < capacity_; ++i) {
            if (data_[i] != nullptr) {
                delete data_[i];
            }
        }
        delete[] data_;
    }

    size_t size() const {
        return size_;
    }

    void add(const T &val) {
        ensureCapacity(size_ + 1);
        data_[size_] = new T(val);
        upheap(size_);
        ++size_;
    }

    T remove() {
        T res = *data_[0];
        delete data_[0];
        data_[0] = data_[size_ - 1];
        data_[size_ - 1] = nullptr;
        --size_;
        downheap(0);
        return res;
    }
};

#endif //HEAP_H

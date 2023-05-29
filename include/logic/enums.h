#ifndef ENUMS_H
#define ENUMS_H

#include <array>

enum class Category {
    ACES,
    TWOS,
    THREES,
    FOURS,
    FIVES,
    SIXES,
    THREE_OF_A_KIND,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    SMALL_STRAIGHT,
    LARGE_STRAIGHT,
    YATHSEE,
    CHANCE,
    size,
};

template<typename Callback>
void forAllCategories(Callback&& callback) {
    for(int c = 0; c < (int)Category::size; ++c) {
        callback((Category)c);
    }
}

#endif
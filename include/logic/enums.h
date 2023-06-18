#ifndef ENUMS_H
#define ENUMS_H

#include <array>
#include <string_view>

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

inline bool isFamily(Category cat) {
    return (int)cat >= (int)Category::ACES && (int)cat <= (int)Category::SIXES;
}

template<typename Callback>
inline static void forAllCategories(Callback&& callback) {
    for(int c = 0; c < (int)Category::size; ++c) {
        callback((Category)c);
    }
}

inline static std::string_view toString(Category cat) {
    using namespace std::literals;
    switch(cat) {
        case Category::ACES: return "ACES"sv;
        case Category::TWOS: return "TWOS"sv;
        case Category::THREES: return "THREES"sv;
        case Category::FOURS: return "FOURS"sv;
        case Category::FIVES: return "FIVES"sv;
        case Category::SIXES: return "SIXES"sv;
        case Category::THREE_OF_A_KIND: return "THREE_OF_A_KIND"sv;
        case Category::FOUR_OF_A_KIND: return "FOUR_OF_A_KIND"sv;
        case Category::FULL_HOUSE: return "FULL_HOUSE"sv;
        case Category::SMALL_STRAIGHT: return "SMALL_STRAIGHT"sv;
        case Category::LARGE_STRAIGHT: return "LARGE_STRAIGHT"sv;
        case Category::YATHSEE: return "YATHSEE"sv;
        case Category::CHANCE: return "CHANCE"sv;
        default: return ""sv;
    }
    return ""sv;
}

#endif
#ifndef PIECE_HEADER
#define PIECE_HEADER

#include "defines.hpp"
#include <cstdint>

typedef uint64_t(*move)(uint64_t);
extern move move_simple_direction[8];
extern move move_knight_direction[8];



struct PIECE{
public:
    static int8_t value;
    static uint8_t index;
    PIECE() = delete;
};



struct SLIDING_PIECE: public PIECE{
public:
    static uint8_t start, end, step;
    SLIDING_PIECE() = delete;
};

struct SINGLE_MOVE_PIECE: public PIECE{
public:

    static move* move_set;
};

struct PAWN: public PIECE{
public:
    constexpr static uint8_t value = 1;
    constexpr static uint8_t index = PAWN_INDEX;
};



struct BISHOP: public SLIDING_PIECE{
public:
    static constexpr int8_t value = 3;
    static constexpr uint8_t index = BISHOP_INDEX;
    static constexpr uint8_t start = 1;
    static constexpr uint8_t end = 7;
    static constexpr uint8_t step = 2;
};
struct ROOK: public SLIDING_PIECE{
public:
    static constexpr int8_t value = 5;
    static constexpr uint8_t index = ROOK_INDEX;
    static constexpr uint8_t start = 0;
    static constexpr uint8_t end = 6;
    static constexpr uint8_t step = 2;
};
struct QUEEN: public SLIDING_PIECE{
public:
    static constexpr int8_t value = 8;
    static constexpr uint8_t index = QUEEN_INDEX;
    static constexpr uint8_t start = 0;
    static constexpr uint8_t end = 7;
    static constexpr uint8_t step = 1;
};



struct KNIGHT: public SINGLE_MOVE_PIECE{
public:
    static constexpr int8_t value = 3;
    static constexpr uint8_t index = KNIGHT_INDEX;
    static constexpr move* move_set = move_knight_direction;
};
struct KING: public SINGLE_MOVE_PIECE{
public:
    static constexpr int8_t value = 0;
    static constexpr uint8_t index = KING_INDEX;
    static constexpr move* move_set = move_simple_direction;
};

struct EMPTY: public PIECE{};


#endif

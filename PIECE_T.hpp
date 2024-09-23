#ifndef I_PIECE_T
#define I_PIECE_T

#include "defines.hpp"
#include <cstdint>
#include "PieceType.hpp"


typedef uint64_t(*move)(uint64_t);
extern move move_simple_direction[8];
extern move move_knight_direction[8];



struct PIECE_T{
public:
    static int8_t value;
    static uint8_t index;
    PIECE_T() = delete;
};


struct SLIDING_PIECE_T: public PIECE_T{
public:
    static uint8_t start, end, step;
    SLIDING_PIECE_T() = delete;
};

struct SINGLE_MOVE_PIECE_T: public PIECE_T{
public:

    static move* move_set;
};

struct PAWN_T: public PIECE_T{
public:
    constexpr static uint8_t value = 1;
    constexpr static PieceType index = PAWN;
};



struct BISHOP_T: public SLIDING_PIECE_T{
public:
    static constexpr int8_t value = 3;
    static constexpr PieceType index = BISHOP;
    static constexpr uint8_t start = 1;
    static constexpr uint8_t end = 7;
    static constexpr uint8_t step = 2;
};
struct ROOK_T: public SLIDING_PIECE_T{
public:
    static constexpr int8_t value = 5;
    static constexpr PieceType index = ROOK;
    static constexpr uint8_t start = 0;
    static constexpr uint8_t end = 6;
    static constexpr uint8_t step = 2;
};
struct QUEEN_T: public SLIDING_PIECE_T{
public:
    static constexpr int8_t value = 8;
    static constexpr PieceType index = QUEEN;
    static constexpr uint8_t start = 0;
    static constexpr uint8_t end = 7;
    static constexpr uint8_t step = 1;
};



struct KNIGHT_T: public SINGLE_MOVE_PIECE_T{
public:
    static constexpr int8_t value = 3;
    static constexpr PieceType index = KNIGHT;
    static constexpr move* move_set = move_knight_direction;
};
struct KING_T: public SINGLE_MOVE_PIECE_T{
public:
    static constexpr int8_t value = 0;
    static constexpr PieceType index = KING;
    static constexpr move* move_set = move_simple_direction;
};


struct EMPTY_T: public PIECE_T{};


#endif

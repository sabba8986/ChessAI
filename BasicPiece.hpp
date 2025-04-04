/*
 * Definition for a BasicPiece struct. A BasicPiece struct contains two simple attributes:
 * Position of the piece 
 * Color of the piece 
 *
 * It does NOT store information about the piece type.
 */

#ifndef BASIC_PIECE
#define BASIC_PIECE


#include "Color.hpp"
#include <cstdint>

struct BasicPiece{
    uint64_t pos;
    Color color;
};


#endif

#ifndef PIECE
#define PIECE

#include "PieceInfo.hpp"
#include <cstdint>


struct Piece{
    PieceInfo info;
    uint64_t pos;
};


#endif

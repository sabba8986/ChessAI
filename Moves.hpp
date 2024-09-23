#ifndef MOVES
#define MOVES

#include "defines.hpp"
#include "BasicPiece.hpp"
#include "Board.hpp"
#include <array>
#include <cstdint>




namespace Moves{
    namespace SlidingPiece{
        //might throw error here because extern declared 
        extern uint64_t get_sliding_space(uint8_t dir, uint64_t pos);
        uint64_t get_full_dir(uint8_t dir, uint64_t pos);
        uint64_t get_moves(uint8_t dir, const BasicPiece& piece, Board& board); 
    }
    namespace King{
        uint64_t get_moves(uint64_t board);
    }
    namespace Knight{
        uint64_t get_moves(uint64_t board);
    }
    namespace Pawn{
        uint64_t black_left_capture(uint64_t pos, Board& board);
        uint64_t black_right_capture(uint64_t pos, Board& board);
        uint64_t white_left_capture(uint64_t pos, Board& board);
        uint64_t white_right_capture(uint64_t pos, Board& board);
        uint64_t black_forward(uint64_t pos, Board& board);
        uint64_t white_forward(uint64_t pos, Board& board);
    }
}




#endif

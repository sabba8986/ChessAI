#include "defines.hpp"
#include "Board.hpp"
#include "move_gen.hpp"
#include <cwchar>
#include <iostream>
#include <chrono>
#include <utility>




extern uint8_t get_diagonal(uint8_t dir, uint8_t pos);



template<typename T>
move_generator<T>::move_generator(Board* board, uint64_t pos, bool is_white): move_generator_t(){
    find_moves(board, pos, is_white);
}




template<typename T>
move_generator<T>::~move_generator(){}




static uint8_t msb_pos(uint64_t board){
    uint8_t index = 0;
    while(board & (~BITBOARD_TOP_LEFT)){
        board <<= 1;
        index++;
    }
    return index;
}




static uint8_t lsb_pos(uint64_t board){
    uint8_t index = 63;
    while(board & (~1)){
        board >>= 1;
        index--;
    }
    return index;
}




static inline uint64_t limit_diagonal(uint64_t bitboard, uint8_t diag){
    return get_diagonal(diag, (diag > 1 && diag < 6) ? lsb_pos(bitboard) : msb_pos(bitboard));
}




template<typename T>
template<typename Q>
std::enable_if_t<std::is_base_of_v<SLIDING_PIECE, Q>> move_generator<T>::find_moves(Board* board, uint64_t pos){
    uint64_t white_pieces = board->all_pieces(true);
    uint64_t black_pieces = board->all_pieces(false);
    for(uint8_t dir = Q::start; dir <= Q::end; dir += Q::step){
        uint64_t all_diagonal_moves = get_diagonal(dir, msb_pos(pos));
        moves |= (all_diagonal_moves ^ limit_diagonal(all_diagonal_moves & white_pieces, dir));
    }
}

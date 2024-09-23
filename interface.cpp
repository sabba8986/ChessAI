#include "Board.hpp"
#include <chrono>
#include <iostream>

Board board;


extern "C" uint64_t generate_all_moves(uint8_t pos){
    return (board.create_generator(pos))->all_legal_moves();
}


extern "C" void make_move(uint8_t start, uint8_t end){
    board.make_move(start, end); 
}

extern "C" uint8_t check(int color){
    return board.is_check(static_cast<Color>(color));
}

extern "C" uint8_t checkmate(int color){
    return board.is_checkmate(static_cast<Color>(color));
}

extern "C" uint64_t get_king_pos(int color){
    return board.get_king(static_cast<Color>(color));
}

extern "C" uint64_t get_pieces(int color){
    return board.get_pieces(static_cast<Color>(color));
}

extern "C" uint64_t all_pieces(){
    return board.all_pieces();
}

extern "C" void reset(){
    board.reset();
}

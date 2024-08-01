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

extern "C" int8_t check(){
    return board.check_status();
}

extern "C" int8_t checkmate(){
    return board.checkmate_status();
}

extern "C" uint64_t get_king_pos(int is_white){
    return board.get_king_pos(is_white);
}

extern "C" void reset(){
    board.reset();
}

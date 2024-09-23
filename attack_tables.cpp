#include "Moves.hpp"
#include "defines.hpp"
#include <iostream>
#include <cstdint>
#include <array>
#include <bitset>




static void print_bitboard(uint64_t bitboard){
    std::string to_print = std::bitset<64>(bitboard).to_string();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            std::cout << to_print[8*i + j];
        }
        std::cout << std::endl;
    }
}



//0 is a sentinel value (originally got SFINAE error because index out of bounds in shift template parameter and SFINAE discarded template so got "function doesn't exit" error for 
//all_moves_knight)
constexpr int SHIFT_SLIDING_AND_KING[] = {
    -8, -7, 1, 9, 8, 7, -1, -9
};
constexpr int SHIFT_KNIGHT[] = {
    10, -6, -15, -17, -10, 6, 15, 17
};

constexpr uint64_t END_SLIDING_AND_KING_MASK[] = {
    BITBOARD_TOP_EDGE, 
    BITBOARD_TOP_EDGE | BITBOARD_RIGHT_EDGE, 
    BITBOARD_RIGHT_EDGE, 
    BITBOARD_RIGHT_EDGE | BITBOARD_BOTTOM_EDGE, 
    BITBOARD_BOTTOM_EDGE, 
    BITBOARD_BOTTOM_EDGE | BITBOARD_LEFT_EDGE, 
    BITBOARD_LEFT_EDGE, 
    BITBOARD_LEFT_EDGE | BITBOARD_TOP_EDGE
};

constexpr uint64_t END_KNIGHT_MASK[] = {
    KNIGHT_1_VALID, 
    KNIGHT_2_VALID,
    KNIGHT_3_VALID,
    KNIGHT_4_VALID, 
    KNIGHT_5_VALID, 
    KNIGHT_6_VALID,
    KNIGHT_7_VALID,
    KNIGHT_8_VALID 
};



constexpr uint64_t get_sliding_space_constexpr(uint8_t dir, uint64_t pos){
    if(pos & END_SLIDING_AND_KING_MASK[dir]){
        return 0;
    } else {
        return SHIFT_SLIDING_AND_KING[dir] < 0 ?
            pos << -SHIFT_SLIDING_AND_KING[dir] : pos >> SHIFT_SLIDING_AND_KING[dir];
    }
}

uint64_t Moves::SlidingPiece::get_sliding_space(uint8_t dir, uint64_t pos){
    if(pos & END_SLIDING_AND_KING_MASK[dir]){
        return 0;
    } else {
        return SHIFT_SLIDING_AND_KING[dir] < 0 ?
            pos << -SHIFT_SLIDING_AND_KING[dir] : pos >> SHIFT_SLIDING_AND_KING[dir];
    }
}

constexpr uint64_t get_knight_space(uint8_t dir, uint64_t pos){
    if(pos & ~END_KNIGHT_MASK[dir]){
        return 0;
    } else {
        return SHIFT_KNIGHT[dir] < 0 ?
            pos << -SHIFT_KNIGHT[dir] : pos >> SHIFT_KNIGHT[dir];
    }
}


template<uint64_t bitboard, uint8_t dir, uint64_t end = END_SLIDING_AND_KING_MASK[dir]>
static constexpr uint64_t all_moves_sliding(){
    if constexpr((bitboard & end) != 0){
        return bitboard;
    } else{ 
        return bitboard | all_moves_sliding<get_sliding_space_constexpr(dir, bitboard), dir>();
    }
};
template<uint64_t bitboard>
static constexpr uint64_t all_moves_knight(){
    uint64_t board = 0;
    for(uint8_t dir = 0; dir < 8; dir++){
        board |= get_knight_space(dir, bitboard);
    }
    return board;
};
template<uint64_t bitboard>
static constexpr uint64_t all_moves_king(){
    uint64_t board = 0;
    for(uint8_t dir = 0; dir < 8; dir++){
        board |= get_sliding_space_constexpr(dir, board);
    }
    return board;
}




template<uint64_t bitboard, uint8_t dir, uint64_t... vals>
static constexpr std::array<uint64_t, 64> sliding_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        return 
            sliding_arr<(bitboard >> 1), dir, vals..., all_moves_sliding<bitboard, dir>() & (~bitboard)>();
    }
};
template<uint64_t bitboard, uint64_t... vals>
static constexpr std::array<uint64_t, 64> knight_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        return knight_arr<(bitboard >> 1), vals..., all_moves_knight<bitboard>() & (~bitboard)>();
    }
};
template<uint64_t bitboard, uint64_t... vals>
static constexpr std::array<uint64_t, 64> king_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        return king_arr<(bitboard >> 1), vals..., all_moves_king<bitboard>() & (~bitboard)>();
    }
}

template<uint64_t bitboard, uint64_t... vals>
static constexpr std::array<uint64_t, 64> white_pawn_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        constexpr uint64_t val = get_sliding_space_constexpr(7, bitboard) | get_sliding_space_constexpr(1, bitboard); 
        return white_pawn_arr<(bitboard >> 1), vals..., val>();
    }
}
template<uint64_t bitboard, uint64_t... vals>
static constexpr std::array<uint64_t, 64> black_pawn_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        constexpr uint64_t val = get_sliding_space_constexpr(3, bitboard) | get_sliding_space_constexpr(5, bitboard); 
        return black_pawn_arr<(bitboard >> 1), vals..., val>();
    }
}


constinit std::array<std::array<uint64_t, 64>, 8> sliding = {
    sliding_arr<BITBOARD_TOP_LEFT, 0>(), 
    sliding_arr<BITBOARD_TOP_LEFT, 1>(), 
    sliding_arr<BITBOARD_TOP_LEFT, 2>(), 
    sliding_arr<BITBOARD_TOP_LEFT, 3>(), 
    sliding_arr<BITBOARD_TOP_LEFT, 4>(), 
    sliding_arr<BITBOARD_TOP_LEFT, 5>(), 
    sliding_arr<BITBOARD_TOP_LEFT, 6>(), 
    sliding_arr<BITBOARD_TOP_LEFT, 7>()
};
constinit std::array<uint64_t, 64> knight = knight_arr<BITBOARD_TOP_LEFT>();
constinit std::array<uint64_t, 64> king = king_arr<BITBOARD_TOP_LEFT>();
constinit std::array<uint64_t, 64> white_pawn = white_pawn_arr<BITBOARD_TOP_LEFT>();
constinit std::array<uint64_t, 64> black_pawn = black_pawn_arr<BITBOARD_TOP_LEFT>();


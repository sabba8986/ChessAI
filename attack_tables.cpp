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
    -8, -7, 1, 9, 8, 7, -1, -9, 0
};
constexpr int SHIFT_KNIGHT[] = {
    10, -6, -15, -17, -10, 6, 15, 17, 0
};



constexpr uint64_t END_SLIDING_AND_KING_MASK[] = {
    BITBOARD_TOP_EDGE, 
    BITBOARD_TOP_EDGE | BITBOARD_RIGHT_EDGE, 
    BITBOARD_RIGHT_EDGE, 
    BITBOARD_RIGHT_EDGE | BITBOARD_BOTTOM_EDGE, 
    BITBOARD_BOTTOM_EDGE, 
    BITBOARD_BOTTOM_EDGE | BITBOARD_LEFT_EDGE, 
    BITBOARD_LEFT_EDGE, 
    BITBOARD_LEFT_EDGE | BITBOARD_TOP_EDGE, 
    0
};
constexpr uint64_t END_KNIGHT_MASK[] = {
    KNIGHT_1_VALID, 
    KNIGHT_2_VALID,
    KNIGHT_3_VALID,
    KNIGHT_4_VALID, 
    KNIGHT_5_VALID, 
    KNIGHT_6_VALID,
    KNIGHT_7_VALID,
    KNIGHT_8_VALID, 
    0
};




template<uint64_t bitboard, int index, uint64_t end = END_SLIDING_AND_KING_MASK[index], int shift = SHIFT_SLIDING_AND_KING[index]>
static constexpr uint64_t all_moves_sliding(){
    if constexpr((bitboard & end) != 0){
        return bitboard;
    } else if constexpr(shift < 0){
        return bitboard | all_moves_sliding<(bitboard << (-1 * shift)), index>();
    } else{
        return bitboard | all_moves_sliding<(bitboard >> shift), index>();
    }
};
template<uint64_t bitboard, int index, uint64_t end = END_KNIGHT_MASK[index], int shift = SHIFT_KNIGHT[index]>
static constexpr uint64_t all_moves_knight(){
    if constexpr(index == 8){
        return 0;
    } else if constexpr((bitboard & end) == 0){
        return all_moves_knight<bitboard, index + 1>();        
    } else if constexpr(shift < 0){
        return (bitboard << (-1 * shift)) | all_moves_knight<bitboard, index + 1>(); 
    } else{
        return (bitboard >> shift) | all_moves_knight<bitboard, index + 1>();
    }
};
template<uint64_t bitboard, int index, uint64_t end = END_SLIDING_AND_KING_MASK[index], int shift = SHIFT_SLIDING_AND_KING[index]>
static constexpr uint64_t all_moves_king(){
    if constexpr(index == 8){
        return 0;
    } else if constexpr((bitboard & end) == 0){
        return all_moves_king<bitboard, index + 1>();        
    } else if constexpr(shift < 0){
        return (bitboard << (-1 * shift)) | all_moves_king<bitboard, index + 1>(); 
    } else{
        return (bitboard >> shift) | all_moves_king<bitboard, index + 1>();
    }
};





template<uint64_t bitboard, int index, uint64_t... vals>
static constexpr std::array<uint64_t, 64> sliding_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        return sliding_arr<(bitboard >> 1), index, vals..., all_moves_sliding<bitboard, index>() & (~bitboard)>();
    }
};
template<uint64_t bitboard, uint64_t... vals>
static constexpr std::array<uint64_t, 64> knight_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        return knight_arr<(bitboard >> 1), vals..., all_moves_knight<bitboard, 0>() & (~bitboard)>();
    }
};
template<uint64_t bitboard, uint64_t... vals>
static constexpr std::array<uint64_t, 64> king_arr(){
    if constexpr(bitboard == 0){
        return {vals...};
    } else{
        return king_arr<(bitboard >> 1), vals..., all_moves_king<bitboard, 0>() & (~bitboard)>();
    }
}


struct Moves{
    static constexpr std::array<std::array<uint64_t, 64>, 8> sliding = {
        sliding_arr<BITBOARD_TOP_LEFT, 0>(), 
        sliding_arr<BITBOARD_TOP_LEFT, 1>(), 
        sliding_arr<BITBOARD_TOP_LEFT, 2>(), 
        sliding_arr<BITBOARD_TOP_LEFT, 3>(), 
        sliding_arr<BITBOARD_TOP_LEFT, 4>(), 
        sliding_arr<BITBOARD_TOP_LEFT, 5>(), 
        sliding_arr<BITBOARD_TOP_LEFT, 6>(), 
        sliding_arr<BITBOARD_TOP_LEFT, 7>()
    };
    static constexpr std::array<uint64_t, 64> knight = knight_arr<BITBOARD_TOP_LEFT>();
    static constexpr std::array<uint64_t, 64> king = king_arr<BITBOARD_TOP_LEFT>();
}; 


inline uint64_t get_diagonal(uint8_t dir, uint8_t pos){
    return Moves::sliding[dir][pos];
    
}

//#include "move_gen.hpp"
//#include "Board.hpp"
#include<cstdint>
#include<iostream>
#include<chrono>
#include<random>
#include<array>

#define BITBOARD_TOP_LEFT 9223372036854775808ULL


extern std::array<std::array<uint64_t, 64>, 8> sliding;


uint8_t lsb_index(uint64_t board){
    uint8_t index = 63;
    uint64_t pos = 1;
    while(!(board & pos)){
        pos <<= 1;
        index--;
    }
    return index;
}





uint8_t msb_index(uint64_t board){
    uint8_t index = 0;
    uint64_t pos = BITBOARD_TOP_LEFT;
    while(!(board & pos)){
        pos >>= 1;
        index++;
    }
    return index;
}


int main(){ 
    std::cout << sliding[2][8];
    return 0;
}

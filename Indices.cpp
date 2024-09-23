#include "Indices.hpp"
#include "defines.hpp"


uint8_t Indices::index_msb(uint64_t board){
    uint8_t index = 0;
    for(uint64_t trav = BITBOARD_TOP_LEFT; trav && !(trav & board); trav >>= 1){
        index++;
    }
    return index;
}

uint8_t Indices::index_lsb(uint64_t board){
    uint8_t index = 63;
    for(uint64_t trav = 1; trav && !(trav & board); trav <<= 1){
        index--;
    }
    return index;
}


uint8_t Indices::index(uint64_t pos){
    return index_msb(pos);
}


uint64_t Indices::msb(uint64_t board){
    uint64_t trav = BITBOARD_TOP_LEFT;
    while(!(trav & board)){
        trav >>= 1;
    }
    return trav;
}


uint64_t Indices::lsb(uint64_t board){
    uint64_t trav = 1;
    while(!(trav & board)){
        trav <<= 1;
    }
    return trav;
}

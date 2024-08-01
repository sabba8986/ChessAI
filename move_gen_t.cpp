#include "move_gen_t.hpp"
#include <cwchar>
#include <iostream>

move_generator_t::move_generator_t(): moves(0){}

uint64_t move_generator_t::all_moves(){
    return moves;
}

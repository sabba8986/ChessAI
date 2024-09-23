#include "move_gen_t.hpp"
#include <cwchar>
#include <iostream>

move_generator_t::move_generator_t(): moves(0){}
move_generator_t::~move_generator_t(){}

uint64_t move_generator_t::all_legal_moves(){return moves;}

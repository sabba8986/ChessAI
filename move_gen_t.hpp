#ifndef MOVE_GEN_T
#define MOVE_GEN_T

#include "defines.hpp"
#include "capture.hpp"
#include <queue>
#include <type_traits>
#include "BasicPiece.hpp"

class Board;


struct move_generator_t{
protected:
    uint64_t moves;

    move_generator_t();
public:
    uint64_t all_legal_moves();

    virtual ~move_generator_t(); 
};



#endif

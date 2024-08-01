#ifndef MOVE_GEN
#define MOVE_GEN

#include "move_gen_t.hpp"
#include "Piece.hpp"



class Board;

template<typename T>
struct move_generator: public move_generator_t{
private: 
public:
    move_generator(Board* board, uint64_t pos, bool is_white);
    ~move_generator() override;
};


#endif


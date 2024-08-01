#ifndef MOVE_GEN_T
#define MOVE_GEN_T

#include "defines.hpp"
#include "capture.hpp"
#include <queue>
#include <type_traits>





struct move_generator_t{
protected:
    uint64_t moves;

    move_generator_t();
    virtual void load_tiles() = 0;
public:
    virtual ~move_generator_t();
    uint64_t all_moves();
};



#endif

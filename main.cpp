#include "move_gen.hpp"
#include "Board.hpp"



int main(){
    Board board;
    std::unique_ptr<move_generator_t> gen = board.create_generator(48); 
    return 0;
}

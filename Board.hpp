#ifndef BOARD
#define BOARD

#include "info.hpp"
#include "move_gen_t.hpp"
#include "Piece.hpp"

#include <cstdint>
#include <memory>

template<typename T> struct move_generator;

class Board{
private:
    //bitboards for each color. Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King}
    uint64_t white[6];
    uint64_t black[6];
    /* first 4 significant bits check check status, least 4 significant signify boolean indicating whether 
     * white goes first. */
    uint8_t status;
    
    uint8_t checking_piece_index;
    uint64_t checking_piece_path;


    uint64_t all_queen_moves(uint64_t pos, bool is_white);

    uint64_t generate_all_moves(uint8_t index, bool is_white, uint64_t pos);
    uint64_t generate_all_moves(uint64_t pos);
    bool non_knight_piece_checking_king(bool is_white);
    bool is_king_in_check(bool is_white);
    bool is_king_in_checkmate(bool is_white);
public:
    Board();
    //White pieces are represented with capital letters, and black pieces are represented with lowercase
    //Knight is represented with N/n
    void print_board();
    static void print_bitboard(uint64_t bitboard);
    tile tile_info(uint64_t bitboard) const;
    std::unique_ptr<move_generator_t> create_generator(uint8_t pos);
    std::unique_ptr<move_generator_t> create_generator(uint8_t index, bool is_white, uint64_t pos);

    //both make_move variants returns the status register before the move was made
    uint8_t make_move(uint64_t old_pos, capture_info new_tile_info);
    uint8_t make_move(uint8_t old_pos, uint8_t new_pos);

    void simulate_move(uint64_t old_pos, tile old_tile, capture_info new_tile_info);
    void undo_move(uint64_t new_pos, capture_info old_tile_info, uint8_t old_status);

    inline uint64_t get_king_pos(bool is_white){return is_white ? white[KING_INDEX] : black[KING_INDEX];}
    inline int8_t check_status(){return (status & CHECK_MASK) ? status & TURN_MASK : -1;}
    inline int8_t checkmate_status(){return (status & CHECKMATE_MASK) ? status & TURN_MASK : -1;}

    inline uint64_t all_pieces(bool is_white){return 
        is_white ? 
        white[0] | white[1] | white[2] | white[3] | white[4] | white[5] :
        black[0] | black[1] | black[2] | black[3] | black[4] | black[5];
    }
    void reset(); 
};




#endif

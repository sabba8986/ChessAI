#ifndef BOARD
#define BOARD

#include "Piece.hpp"
#include "Color.hpp"
#include "move_gen_t.hpp"
#include "move_gen.hpp"


#include <cstdint>
#include <memory>


class Board{
private:
    //bitboards for each color. Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King}
    uint64_t white[6];
    uint64_t black[6];
    /* first 4 significant bits check check status, least 4 significant signify boolean indicating whether 
    * white goes first. */
    uint8_t status;
    std::array<BasicPiece, 64> threat_list;

    uint64_t white_threats;
    uint64_t black_threats;

    uint64_t checking_piece_threat_moves;


    std::unique_ptr<move_generator_t> create_generator(const Piece& piece);
    uint8_t make_move(const Piece& piece1, const Piece& piece2);

    uint64_t generate_all_threat_moves(const Piece& piece);
    uint64_t generate_all_check_moves(const Piece& piece);

    void regenerate_threats_color(Color color);
    inline void regenerate_threats(){
        regenerate_threats_color(WHITE);
        regenerate_threats_color(BLACK);
    }

    uint64_t all_moves_in_all_dirs(BasicPiece piece) const;
    bool is_pinned_piece(uint8_t threat_dir, const Piece& piece);
    inline uint64_t get_threats(Color color) const{
        return color == WHITE ? black_threats : white_threats;
    }
    inline void toggle_piece(const Piece& piece){
        (piece.info.color == Color::WHITE ? white : black)[piece.info.type] ^= piece.pos;
    }

    void update_threat_list(BasicPiece piece);
    void update_for_each(uint64_t pieces, uint64_t pos_to_update, bool turn_on);


    uint8_t get_dir_between_spaces(uint64_t space_1, uint64_t space_2) const;
public:
    Board();
    //White pieces are represented with capital letters, and black pieces are represented with lowercase
    //Knight is represented with N/n
    void print_board();
    static void print_bitboard(uint64_t bitboard);
    PieceInfo tile_info(uint64_t bitboard);
    uint64_t all_legal_moves(uint8_t pos); 

    //both make_move variants returns the status register before the move was made
    uint8_t make_move(uint8_t old_pos, uint8_t new_pos); 
    std::unique_ptr<move_generator_t> create_generator(uint8_t pos);

    void change_piece_color(const Piece& piece);
    inline uint64_t get_king(Color color){
        return color == Color::WHITE ? white[KING_INDEX] : black[KING_INDEX];
    }

    inline uint64_t* get_allies(Color color){
        return color == Color::WHITE ? white : black;
    }

    inline uint64_t* get_opps(Color color){
        return get_allies(~color);
    }

    inline void set_check(){
        status |= CHECK;
    }
    inline void set_checkmate(){
        status |= CHECKMATE;
    }

    inline void change_turn(){
        status ^= TURN;
    }
    inline bool is_check(Color color) const{
        return ((status & CHECK) != 0) && (static_cast<Color>(status & TURN) == color);
    }
    
    inline bool is_checkmate(Color color) const{
        return ((status & CHECKMATE) != 0) && (static_cast<Color>(status & TURN) == color);
    }

    inline uint64_t get_pieces(Color color) const{
        return color == Color::WHITE ? white[0] | white[1] | white[2] | white[3] | white[4] | white[5] :
                                       black[0] | black[1] | black[2] | black[3] | black[4] | black[5];
    }
    inline uint64_t get_board(Color color, PieceType type) const{
        return (color == Color::WHITE ? white : black)[type];
    }
    inline uint64_t all_pieces() const{
        return get_pieces(Color::WHITE) | get_pieces(Color::BLACK);
    }

    void reset(); 

    template<typename T>
    friend struct move_generator;
};




#endif

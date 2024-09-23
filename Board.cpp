#include "move_gen.hpp"
#include "Board.hpp"
#include "Moves.hpp"
#include "PieceType.hpp"
#include "Indices.hpp"
#include <iostream>
#include <bitset>



static constexpr std::array<uint8_t, 4> upper_dirs = {6, 7, 0, 1};
static constexpr std::array<uint8_t, 4> lower_dirs = {2, 3, 4, 5};
static constexpr PieceType types[] = {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

static uint8_t opp_dir(uint8_t dir){
    return (dir + 4) % 8;
}

std::unique_ptr<move_generator_t> Board::create_generator(const Piece& piece_){
    BasicPiece piece = {piece_.pos, piece_.info.color};
    switch(piece_.info.type){
        case EMPTY:
            return std::make_unique<move_generator<EMPTY_T>>(piece, *this);
        case PAWN:
            return std::make_unique<move_generator<PAWN_T>>(piece, *this);
        case ROOK:
            return std::make_unique<move_generator<ROOK_T>>(piece, *this);
        case BISHOP:
            return std::make_unique<move_generator<BISHOP_T>>(piece, *this);
        case KNIGHT:
            return std::make_unique<move_generator<KNIGHT_T>>(piece, *this);
        case QUEEN:
            return std::make_unique<move_generator<QUEEN_T>>(piece, *this);
        case KING:
            return std::make_unique<move_generator<KING_T>>(piece, *this);
    }
}


std::unique_ptr<move_generator_t> Board::create_generator(uint8_t pos){
    return create_generator({tile_info(BITBOARD_TOP_LEFT >> pos), BITBOARD_TOP_LEFT >> pos});
}

uint64_t Board::all_legal_moves(uint8_t pos){
    return create_generator(pos)->all_legal_moves();
}

uint64_t Board::generate_all_threat_moves(const Piece& piece_){
    BasicPiece piece = {piece_.pos, piece_.info.color}; 
    switch(piece_.info.type){
        case EMPTY:
            return 0;
        case PAWN:
            return move_generator<PAWN_T>::all_threat_moves(piece, *this);
        case ROOK:
            return move_generator<ROOK_T>::all_threat_moves(piece, *this);
        case BISHOP:
            return move_generator<BISHOP_T>::all_threat_moves(piece, *this); 
        case KNIGHT:
            return move_generator<KNIGHT_T>::all_threat_moves(piece, *this);
        case QUEEN:
            return move_generator<QUEEN_T>::all_threat_moves(piece, *this);
        case KING:
            return move_generator<KING_T>::all_threat_moves(piece, *this); 
    }

}


uint64_t Board::generate_all_check_moves(const Piece& piece_){
    BasicPiece piece = {piece_.pos, piece_.info.color}; 
    switch(piece_.info.type){
        case EMPTY:
            return 0;
        case PAWN:
            return move_generator<PAWN_T>::all_check_moves(piece, *this);
        case ROOK:
            return move_generator<ROOK_T>::all_check_moves(piece, *this);
        case BISHOP:
            return move_generator<BISHOP_T>::all_check_moves(piece, *this); 
        case KNIGHT:
            return move_generator<KNIGHT_T>::all_check_moves(piece, *this);
        case QUEEN:
            return move_generator<QUEEN_T>::all_check_moves(piece, *this);
        case KING:
            return move_generator<KING_T>::all_check_moves(piece, *this); 
    }

}


void Board::regenerate_threats_color(Color color){
    uint64_t& ally_threats = (color == WHITE ? white_threats : black_threats);
    ally_threats = 0;
    const uint64_t* allies = get_allies(color);
    for(PieceType type: types){
        uint64_t board = allies[type];
        for(uint64_t trav = BITBOARD_TOP_LEFT; trav; trav >>= 1){
            if(trav & board){
                ally_threats |= generate_all_threat_moves({{type, color}, trav});
            }
        }
    }
}


uint8_t Board::get_dir_between_spaces(uint64_t space_1, uint64_t space_2) const{
    using namespace Moves::SlidingPiece;
    const std::array<uint8_t, 4>& dirs = (space_2 > space_1) ? upper_dirs : lower_dirs;
    uint64_t all_pieces = this->all_pieces();
    for(const uint8_t dir: dirs){
        uint64_t overlap = (get_full_dir(dir, space_1) & get_full_dir(opp_dir(dir), space_2));
        if(overlap){
            return overlap & all_pieces ? NO_THREAT_DIR : dir;
        }
    }
    return NO_THREAT_DIR;
}


bool Board::is_pinned_piece(uint8_t threat_dir, const Piece& piece){
    Color color = piece.info.color;
    PieceType type = piece.info.type;
    const uint64_t* opposing_color = get_opps(color);
    uint64_t possible_threats = 
        get_board(~color, PieceType::QUEEN) | 
        get_board(~color, (threat_dir % 2) ? PieceType::BISHOP : PieceType::ROOK);
    toggle_piece(piece);
    uint64_t threat_dir_moves = Moves::SlidingPiece::get_moves(
        threat_dir, {get_king(color), color}, *this
    );
    toggle_piece(piece);
    return (threat_dir_moves & possible_threats) != 0;
}

//returns:
//for tile_val: 0 if can't move into that space, 1 if the space is empty, or -1 if the space contains a piece of the opposing color
//for capture_index, -1 if tile_val is not -1, or the index of the opposing piece if tile_val if the
PieceInfo Board::tile_info(uint64_t bitboard){
    for(PieceType type: types){
        if(get_board(WHITE, type) & bitboard){
            return {type, WHITE};
        } else if(get_board(BLACK, type) & bitboard){
            return {type, BLACK};
        }
    }
    return {EMPTY, BLACK};
} 


Board::Board(){
    reset();
}


//need to replace macros with enum valear
void Board::print_board(){
    uint64_t trav = BITBOARD_TOP_LEFT;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(trav & white[PAWN_INDEX]) std::cout << "P";
            else if(trav & black[PAWN_INDEX]) std::cout << "p";
            else if(trav & white[ROOK_INDEX]) std::cout << "R";
            else if(trav & black[ROOK_INDEX]) std::cout << "r";
            else if(trav & white[KNIGHT_INDEX]) std::cout << "N";
            else if(trav & black[KNIGHT_INDEX]) std::cout << "n";
            else if(trav & white[BISHOP_INDEX]) std::cout << "B";
            else if(trav & black[BISHOP_INDEX]) std::cout << "b";
            else if(trav & white[QUEEN_INDEX]) std::cout << "Q";
            else if(trav & black[QUEEN_INDEX]) std::cout << "q";
            else if(trav & white[KING_INDEX]) std::cout << "K";
            else if(trav & black[KING_INDEX]) std::cout << "k";
            else std::cout << " ";
            trav >>= 1;
        }
        if(i < 7) std::cout << std::endl;
    }
    std::cout << std::endl;
}




void Board::print_bitboard(uint64_t bitboard){
    std::string to_print = std::bitset<64>(bitboard).to_string();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            std::cout << to_print[8*i + j];
        }
        std::cout << std::endl;
    }
}



uint8_t Board::make_move(uint8_t start_pos, uint8_t end_pos){
    uint64_t old_bitboard = BITBOARD_TOP_LEFT >> start_pos;
    uint64_t new_bitboard = BITBOARD_TOP_LEFT >> end_pos;
    return make_move({tile_info(old_bitboard), old_bitboard}, 
                            {tile_info(new_bitboard), new_bitboard});
}




uint8_t Board::make_move(const Piece& a_piece, const Piece& d_piece){
    //changing bitboards
    Color a_color = a_piece.info.color;
    Color d_color = d_piece.info.color;
    PieceType a_type = a_piece.info.type;
    PieceType d_type = d_piece.info.type; 
    get_allies(a_color)[a_type] ^= (a_piece.pos | d_piece.pos);
    if(d_type != EMPTY){
        get_allies(d_color)[d_type] &= ~d_piece.pos;
    } 
    //updating threat list
    /*std::cout << "White threats:\n";
    print_bitboard(white_threats);
    std::cout << "\n\nBlack threats:\n";
    print_bitboard(black_threats);
    */
    const Piece& new_a_piece = {a_piece.info, d_piece.pos};
    checking_piece_threat_moves = generate_all_check_moves(new_a_piece);
    std::cout << "The checking piece (if there is a check) is making moves: " << std::endl;
    print_bitboard(checking_piece_threat_moves);
    std::cout << std::endl;
    if(generate_all_threat_moves(new_a_piece) & get_king(~new_a_piece.info.color)){  
        change_piece_color(new_a_piece);
        regenerate_threats();
        change_piece_color(new_a_piece);
        set_check();
        std::cout << "Check!" << std::endl;
    } else{
        regenerate_threats();
    }
    change_turn();
    return 0;
}


void Board::change_piece_color(const Piece& piece){
    get_allies(piece.info.color)[piece.info.type] ^= piece.pos;
    get_opps(piece.info.color)[piece.info.type] ^= piece.pos;
}

void Board::reset(){
    status = 1;
    white[PAWN_INDEX] = INIT_WHITE_PAWNS;
    white[ROOK_INDEX] = INIT_WHITE_ROOKS;
    white[KNIGHT_INDEX] = INIT_WHITE_KNIGHTS;
    white[BISHOP_INDEX] = INIT_WHITE_BISHOPS;
    white[QUEEN_INDEX] = INIT_WHITE_QUEEN;
    white[KING_INDEX] = INIT_WHITE_KING;

    black[PAWN_INDEX] = INIT_BLACK_PAWNS;
    black[ROOK_INDEX] = INIT_BLACK_ROOKS;
    black[KNIGHT_INDEX] = INIT_BLACK_KNIGHTS;
    black[BISHOP_INDEX] = INIT_BLACK_BISHOPS;
    black[QUEEN_INDEX] = INIT_BLACK_QUEEN;
    black[KING_INDEX] = INIT_BLACK_KING;
    regenerate_threats();
}

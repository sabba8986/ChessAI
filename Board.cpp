#include "move_gen.hpp"
#include "Board.hpp"
#include <iostream>
#include <bitset>


//returns:
//for tile_val: 0 if can't move into that space, 1 if the space is empty, or -1 if the space contains a piece of the opposing color
//for capture_index, -1 if tile_val is not -1, or the index of the opposing piece if tile_val if the
tile Board::tile_info(uint64_t bitboard) const{
    if(bitboard == UINT64_MAX) return {INVALID_TILE, false};
    for(int8_t i = 0; i < 6; i++){
        if(bitboard & white[i]){
            return {i, true};
        } else if(bitboard & black[i]){
            return {i, false};
        }
    }
    return {-1, false};
} 


Board::Board(): status(8){
    reset();
}



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


std::unique_ptr<move_generator_t> Board::create_generator(uint8_t pos){
   tile tile = tile_info(BITBOARD_TOP_LEFT >> pos); 
   if(tile.is_white == (bool)(status & TURN_MASK)){
       return std::make_unique<move_generator<EMPTY>>(this, pos, tile.is_white);
   } 
   switch(tile.index){
       case PAWN_INDEX:
           return std::make_unique<move_generator<PAWN>>(this, pos, tile.is_white);
       case ROOK_INDEX:
           return std::make_unique<move_generator<ROOK>>(this, pos, tile.is_white); 
       case KNIGHT_INDEX:
           return std::make_unique<move_generator<KNIGHT>>(this, pos, tile.is_white); 
       case BISHOP_INDEX:
           return std::make_unique<move_generator<BISHOP>>(this, pos, tile.is_white);
       case QUEEN_INDEX:
           return std::make_unique<move_generator<QUEEN>>(this, pos, tile.is_white);
       case KING_INDEX:
           return std::make_unique<move_generator<KING>>(this, pos, tile.is_white);
       default:
           return std::make_unique<move_generator<EMPTY>>(this, pos, true);
   }
}

std::unique_ptr<move_generator_t> Board::create_generator(uint8_t index, bool is_white, uint64_t pos){ 
   if(is_white == (bool)(status & TURN_MASK)){
       return std::make_unique<move_generator<EMPTY>>(this, pos, is_white);
   } 
   switch(index){
       case PAWN_INDEX:
           return std::make_unique<move_generator<PAWN>>(this, pos, is_white);
       case ROOK_INDEX:
           return std::make_unique<move_generator<ROOK>>(this, pos, is_white); 
       case KNIGHT_INDEX:
           return std::make_unique<move_generator<KNIGHT>>(this, pos, is_white); 
       case BISHOP_INDEX:
           return std::make_unique<move_generator<BISHOP>>(this, pos, is_white);
       case QUEEN_INDEX:
           return std::make_unique<move_generator<QUEEN>>(this, pos, is_white);
       case KING_INDEX:
           return std::make_unique<move_generator<KING>>(this, pos, is_white);
       default:
           return std::make_unique<move_generator<EMPTY>>(this, pos, true);
   }
}

void Board::simulate_move(uint64_t old_pos, tile old_tile, capture_info new_tile_info){
    (old_tile.is_white ? white : black)[old_tile.index] &= ~old_pos;
    (old_tile.is_white ? white : black)[old_tile.index] |= new_tile_info.bitboard;
    if(new_tile_info.index != -1){
        (old_tile.is_white ? black : white)[new_tile_info.index] &= ~(new_tile_info.bitboard);
    }
}


uint8_t Board::make_move(uint64_t old_pos, capture_info new_tile_info){
    tile old_tile = this->tile_info(old_pos);
    uint8_t prev_status = status;
    simulate_move(old_pos, old_tile, new_tile_info);
        if(is_king_in_check(old_tile.is_white ? false : true)){
        status |= CHECK_MASK;
        if(is_king_in_checkmate(old_tile.is_white ? false : true)){
            std::cout << "Not supposed to be here" << std::endl;
            status |= CHECKMATE_MASK;
        }
    }
    status ^= TURN_MASK; 
    return prev_status;
}


uint8_t Board::make_move(uint8_t start_pos, uint8_t end_pos){
    uint64_t new_board = BITBOARD_TOP_LEFT >> end_pos;
    return this->make_move(BITBOARD_TOP_LEFT >> start_pos, {new_board, this->tile_info(new_board).index});
}


void Board::undo_move(uint64_t old_pos, capture_info new_tile_info, uint8_t old_status){
    tile after_capture = this->tile_info(new_tile_info.bitboard);
    (after_capture.is_white ? white : black)[after_capture.index] &= ~new_tile_info.bitboard;
    (after_capture.is_white ? white : black)[after_capture.index] |= old_pos;
    if(new_tile_info.index != -1){
        (after_capture.is_white ? black : white)[new_tile_info.index] |= new_tile_info.bitboard;
    }
    status = old_status; 
}





uint64_t Board::generate_all_moves(uint8_t index, bool is_white, uint64_t pos){
    switch(index){
        case PAWN_INDEX:
            return move_generator<PAWN>::all_moves<>(this, pos, is_white);
        case BISHOP_INDEX:
            return move_generator<BISHOP>::all_moves<>(this, pos, is_white);
        case ROOK_INDEX:
            return move_generator<ROOK>::all_moves<>(this, pos, is_white);
        case QUEEN_INDEX:
            return move_generator<QUEEN>::all_moves<>(this, pos, is_white);
        case KNIGHT_INDEX:
            return move_generator<KNIGHT>::all_moves<>(this, pos, is_white);
        case KING_INDEX:
            return move_generator<KING>::all_moves<>(this, pos, is_white);
        default:
            return 0;
    }
}

uint64_t Board::generate_all_moves(uint64_t pos){
    tile tile = tile_info(pos);
    switch(tile.index){
        case PAWN_INDEX:
            return move_generator<PAWN>::all_moves<>(this, pos, tile.is_white);
        case BISHOP_INDEX:
            return move_generator<BISHOP>::all_moves<>(this, pos, tile.is_white);
        case ROOK_INDEX:
            return move_generator<ROOK>::all_moves<>(this, pos, tile.is_white);
        case QUEEN_INDEX:
            return move_generator<QUEEN>::all_moves<>(this, pos, tile.is_white);
        case KNIGHT_INDEX:
            return move_generator<KNIGHT>::all_moves<>(this, pos, tile.is_white);
        case KING_INDEX:
            return move_generator<KING>::all_moves<>(this, pos, tile.is_white);
        default:
            return 0;
    }
}



bool Board::non_knight_piece_checking_king(bool is_white){
    uint64_t king_pos = is_white ? white[KING_INDEX] : black[KING_INDEX];
    uint64_t all_possible_attack_tiles = move_generator<QUEEN>::all_moves(this, king_pos, is_white); 
    for(uint64_t trav = BITBOARD_TOP_LEFT; trav; trav >>= 1){
        if(trav & all_possible_attack_tiles){
            tile attack_tile = tile_info(trav);
            if(attack_tile.index != EMPTY_TILE && attack_tile.is_white != is_white){
                if(king_pos & generate_all_moves(attack_tile.index, attack_tile.is_white, trav)){
                    return true;
                }
            }
        }
    }
    return false;
}


bool Board::is_king_in_check(bool is_white){
    uint64_t all_possible_knight_pos = 
        move_generator<KNIGHT>::all_moves(this, is_white ? white[KING_INDEX] : black[KING_INDEX], is_white);
    for(uint64_t trav = BITBOARD_TOP_LEFT; trav; trav >>= 1){
        if(trav & all_possible_knight_pos){
            tile attack_tile = tile_info(trav);
            if(attack_tile.index == KNIGHT_INDEX && attack_tile.is_white != is_white){
                return true;
            }
        }
    }
    return non_knight_piece_checking_king(is_white);
}


bool Board::is_king_in_checkmate(bool is_white){
    uint64_t all_pieces = is_white ? black[0] | black[1] | black[2] | black[3] | black[4] | black[5] : 
        white[0] | white[1] | white[2] | white[3] | white[4] | white[5];
    uint8_t counter = 0;
    for(uint64_t trav = BITBOARD_TOP_LEFT; trav; trav >>= 1, counter++){
        if(trav & all_pieces){
            capture_info info;
            std::unique_ptr<move_generator_t> gen = create_generator(counter);
            while((info = gen->next_move()).bitboard != UINT64_MAX){
                simulate_move(trav, tile_info(trav), info);
                bool is_still_in_check = is_king_in_check(is_white);
                undo_move(trav, info, status);
                if(!is_still_in_check){
                    return false;
                }
            }
        }
    }
    return true;
}


void Board::reset(){
    status = 8;
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

}

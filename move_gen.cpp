#include "defines.hpp"
#include "Board.hpp"
#include "Moves.hpp"
#include "move_gen.hpp"
#include <cwchar>
#include <iostream>
#include <chrono>
#include <utility>




template<typename T>
move_generator<T>::move_generator(const BasicPiece& piece, Board& board): move_generator_t(){
    if constexpr(!std::is_same_v<EMPTY_T, T>){ 
        moves = board.is_check(piece.color) ? 
            all_legal_moves_check(piece, board) : 
            all_legal_moves_no_check(piece, board);
    }
}

template<typename T>
move_generator<T>::~move_generator(){}



template<typename T>
static bool can_move_in_dir_sliding(uint8_t dir){
    if constexpr(std::is_same_v<BISHOP_T, T>){
        return dir % 2 == 1;
    } else if constexpr(std::is_same_v<ROOK_T, T>){
        return dir % 2 == 0;
    } 
    else{
        return true;
    }
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::single_move(uint8_t dir, const BasicPiece& piece, Board& board) requires std::derived_from<Q, SLIDING_PIECE_T>{
    using namespace Moves::SlidingPiece;
    if constexpr(std::is_same_v<QUEEN_T, Q>) return get_moves(dir, piece, board);
    else{
        return dir % 2 == Q::start ? get_moves(dir, piece, board) : 0;
    }
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::single_move(uint8_t dir, const BasicPiece& piece, Board& board) requires std::same_as<KNIGHT_T, Q>{ 
    return 0;
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::single_move(uint8_t dir, const BasicPiece& piece, Board& board) requires std::same_as<PAWN_T, Q>{
    using namespace Moves::Pawn;
    if(piece.color == WHITE){
        if(dir == 7) return white_left_capture(piece.pos, board);
        else if(dir == 0) return white_forward(piece.pos, board);
        else if(dir == 1) return white_right_capture(piece.pos, board);
    } else{
        if(dir == 3) return black_right_capture(piece.pos, board);
        else if(dir == 4) return black_forward(piece.pos, board);
        else if(dir == 5) return black_left_capture(piece.pos, board);
    }
    return 0;
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_threat_moves(const BasicPiece& piece, Board& board) requires std::derived_from<Q, SLIDING_PIECE_T>{
    using namespace Moves::SlidingPiece; 
    uint64_t all_moves = 0;
    for(uint8_t dir = Q::start; dir <= Q::end; dir += Q::step){
        all_moves |= get_moves(dir, piece, board);
    }
    return all_moves;
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_threat_moves(const BasicPiece& piece, Board& board) requires std::derived_from<Q, SINGLE_MOVE_PIECE_T>{
    if constexpr(std::is_same_v<KING_T, Q>){
        return Moves::King::get_moves(piece.pos) & ~(board.get_pieces(piece.color));
    } else{
        return Moves::Knight::get_moves(piece.pos) & ~(board.get_pieces(piece.color));
    }
} 

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_threat_moves(const BasicPiece& piece, Board& board) requires std::same_as<PAWN_T, Q>{
    using namespace Moves::SlidingPiece;
    if(piece.color == WHITE){
        return get_sliding_space(7, piece.pos) | get_sliding_space(1, piece.pos);
    }
    return get_sliding_space(3, piece.pos) | get_sliding_space(5, piece.pos); 
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_moves(const BasicPiece& piece, Board& board) requires (!std::same_as<PAWN_T, Q>){
    return all_threat_moves(piece, board);
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_moves(const BasicPiece& piece, Board& board) requires std::same_as<PAWN_T, Q>{
    using namespace Moves::Pawn;
    if(piece.color == WHITE){
        return white_left_capture(piece.pos, board) | white_right_capture(piece.pos, board) | 
            white_forward(piece.pos, board);
    }
    return black_left_capture(piece.pos, board) | black_right_capture(piece.pos, board) | 
            black_forward(piece.pos, board);
}


template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_check_moves(const BasicPiece& piece, Board& board) requires std::derived_from<SLIDING_PIECE_T, Q>{
    using namespace Moves::SlidingPiece;
    return get_moves(board.get_dir_between_spaces(piece.pos, board.get_king(~piece.color)), piece, board);
}


template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_check_moves(const BasicPiece& piece, Board& board) requires (!std::derived_from<SLIDING_PIECE_T, Q>){
    return piece.pos;
}



template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_legal_moves_no_check(const BasicPiece& piece, Board& board) requires (!std::same_as<KING_T, Q>){
    uint8_t threat_dir = board.get_dir_between_spaces(board.get_king(piece.color), piece.pos);
    if(threat_dir != NO_THREAT_DIR){
        if(board.is_pinned_piece(threat_dir, {{Q::index, piece.color}, piece.pos})){
            return single_move(threat_dir, piece, board);
        }        
    }
    return all_moves(piece, board);
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_legal_moves_no_check(const BasicPiece& piece, Board& board) requires (std::same_as<KING_T, Q>){
    return all_moves(piece, board) & ~board.checking_piece_threat_moves;
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_legal_moves_check(const BasicPiece& piece, Board& board) requires (!std::same_as<KING_T, Q>){
    return all_moves(piece, board) & board.checking_piece_threat_moves; 
}

template<typename T>
template<typename Q>
uint64_t move_generator<T>::all_legal_moves_check(const BasicPiece& piece, Board& board) requires std::same_as<KING_T, Q>{
    return all_legal_moves_no_check(piece, board);
}


template struct move_generator<EMPTY_T>;
template struct move_generator<PAWN_T>;
template struct move_generator<ROOK_T>;
template struct move_generator<KNIGHT_T>;
template struct move_generator<BISHOP_T>;
template struct move_generator<QUEEN_T>;
template struct move_generator<KING_T>;
 
template uint64_t move_generator<PAWN_T>::all_threat_moves<>(const BasicPiece& piece, Board& board);
template uint64_t move_generator<ROOK_T>::all_threat_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<KNIGHT_T>::all_threat_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<BISHOP_T>::all_threat_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<QUEEN_T>::all_threat_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<KING_T>::all_threat_moves<>(const BasicPiece&, Board&);


template uint64_t move_generator<PAWN_T>::all_check_moves<>(const BasicPiece& piece, Board& board);
template uint64_t move_generator<ROOK_T>::all_check_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<KNIGHT_T>::all_check_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<BISHOP_T>::all_check_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<QUEEN_T>::all_check_moves<>(const BasicPiece&, Board&);
template uint64_t move_generator<KING_T>::all_check_moves<>(const BasicPiece&, Board&);


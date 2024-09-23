#ifndef MOVE_GEN
#define MOVE_GEN

#include "move_gen_t.hpp"
#include "BasicPiece.hpp"
#include "PIECE_T.hpp"
#include <cstdlib>
#include <concepts>


class Board;


template<typename T>
struct move_generator: public move_generator_t{

    template<typename Q = T>
    uint64_t single_move(uint8_t dir, const BasicPiece& piece, Board& board) requires std::derived_from<Q, SLIDING_PIECE_T>; 
    template<typename Q = T>
    uint64_t single_move(uint8_t dir, const BasicPiece& piece, Board& board) requires std::same_as<KNIGHT_T, Q>;
    template<typename Q = T>
    uint64_t single_move(uint8_t dir, const BasicPiece& piece, Board& board) requires std::same_as<PAWN_T, Q>;

    template<typename Q = T>
    static uint64_t all_check_moves(const BasicPiece& piece, Board& board) requires std::derived_from<SLIDING_PIECE_T, Q>;
    template<typename Q = T>
    static uint64_t all_check_moves(const BasicPiece& piece, Board& board) requires (!std::derived_from<SLIDING_PIECE_T, Q>);
  

    template<typename Q = T>
    uint64_t all_moves(const BasicPiece& piece, Board& board) requires (!std::same_as<PAWN_T, Q>);
    template<typename Q = T>
    uint64_t all_moves(const BasicPiece& piece, Board& board) requires std::same_as<PAWN_T, Q>;

    template<typename Q = T>
    uint64_t all_legal_moves_no_check(const BasicPiece& piece, Board& board) requires (!std::same_as<KING_T, Q>);
    template<typename Q = T>
    uint64_t all_legal_moves_no_check(const BasicPiece& piece, Board& board) requires (std::same_as<KING_T, Q>);

    template<typename Q = T>
    uint64_t all_legal_moves_check(const BasicPiece& piece, Board& board) requires (!std::same_as<KING_T, Q>);
    template<typename Q = T>
    uint64_t all_legal_moves_check(const BasicPiece& piece, Board& board) requires std::same_as<KING_T, Q>;

public:

    template<typename Q = T>
    static uint64_t all_threat_moves(const BasicPiece& piece, Board& board) requires std::derived_from<Q, SLIDING_PIECE_T>;
    template<typename Q = T>
    static uint64_t all_threat_moves(const BasicPiece& piece, Board& board) requires std::derived_from<Q, SINGLE_MOVE_PIECE_T>;
    template<typename Q = T>
    static uint64_t all_threat_moves(const BasicPiece& piece, Board& board) requires std::same_as<PAWN_T, Q>;

    move_generator(const BasicPiece& piece, Board& board);
    ~move_generator() override;


};


#endif

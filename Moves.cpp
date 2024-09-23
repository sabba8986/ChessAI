#include "Moves.hpp"
#include "Indices.hpp"
#include "Color.hpp"



extern const std::array<std::array<uint64_t, 64>, 8> sliding;
extern const std::array<uint64_t, 64> knight;
extern const std::array<uint64_t, 64> king;
extern const std::array<uint64_t, 64> white_pawn;
extern const std::array<uint64_t, 64> black_pawn;



static uint64_t limit_dir(uint8_t dir, uint64_t board){
    using namespace Indices;
    if(!board) return 0;
    return sliding[dir][(dir > 1 && dir < 6) ? index_msb(board) : index_lsb(board)]; 
}

uint64_t Moves::SlidingPiece::get_full_dir(uint8_t dir, uint64_t pos){
    using namespace Indices;
    return sliding[dir][index(pos)];
}

uint64_t Moves::SlidingPiece::get_moves(uint8_t dir, const BasicPiece& piece, Board& board){ 
    using namespace Indices;
    uint64_t all_moves = sliding[dir][index(piece.pos)];
    all_moves ^= limit_dir(dir, all_moves & board.all_pieces());
    return all_moves &= ~board.get_pieces(piece.color);
}

uint64_t Moves::King::get_moves(uint64_t pos){
    using namespace Indices;
    return king[index(pos)];
}

uint64_t Moves::Knight::get_moves(uint64_t pos){
    using namespace Indices;
    return knight[index(pos)];
}

uint64_t Moves::Pawn::black_left_capture(uint64_t pos, Board& board){
    using namespace Moves::SlidingPiece;
    return get_sliding_space(5, pos) & board.get_pieces(WHITE);
}

uint64_t Moves::Pawn::black_right_capture(uint64_t pos, Board& board){
    using namespace Moves::SlidingPiece;
    return get_sliding_space(3, pos) & board.get_pieces(WHITE);
}

uint64_t Moves::Pawn::white_left_capture(uint64_t pos, Board& board){
    using namespace Moves::SlidingPiece;
    return get_sliding_space(7, pos) & board.get_pieces(BLACK);
}

uint64_t Moves::Pawn::white_right_capture(uint64_t pos, Board& board){
    using namespace Moves::SlidingPiece;
    return get_sliding_space(1, pos) & board.get_pieces(BLACK);
}

uint64_t Moves::Pawn::white_forward(uint64_t pos, Board& board){
    using namespace Moves::SlidingPiece;
    uint64_t all_spaces = ~board.all_pieces();
    uint64_t moves = get_sliding_space(0, pos) & all_spaces;
    return moves && (pos & WHITE_PAWN_ROW) ? moves | (get_sliding_space(0, moves) & all_spaces) : moves; 
}

uint64_t Moves::Pawn::black_forward(uint64_t pos, Board& board){
    using namespace Moves::SlidingPiece;
    uint64_t all_spaces = ~board.all_pieces();
    uint64_t moves = get_sliding_space(4, pos) & all_spaces;
    return moves && (pos & BLACK_PAWN_ROW) ? moves | (get_sliding_space(4, moves) & all_spaces) : moves; 
}




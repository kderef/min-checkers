#include <stdint.h>

// SOURCES:
// (chess bitboards used for inspiration)
// - https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/rep.html
// - https://healeycodes.com/visualizing-chess-bitboards

// 8x8 board
typedef struct {
    uint32_t white;
    uint32_t black;
    uint32_t kings;
} Board;

Board board = {
    .kings = 0,
    .white = 0xFFF00000,
    .black = 0x00000FFF
};

typedef enum {
    PIECE_NONE = 0,
    PIECE_WHITE,
    PIECE_WHITE_KING,
    PIECE_BLACK,
    PIECE_BLACK_KING
} Piece;

Piece board_get(unsigned x, unsigned y) {
    // x = 0 | 1 | 2 | 3
    // y = 0 | 1 | 2 | 3
    
    const uint32_t mask = 1u << (y * 4 + x);

    if (board.kings & mask) {
        return (board.black & mask)? PIECE_BLACK_KING : PIECE_WHITE_KING;
    }
    if (board.black & mask) return PIECE_BLACK;
    if (board.white & mask) return PIECE_WHITE; 

    return PIECE_NONE;
}

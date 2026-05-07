#include <stdint.h>

// SOURCES:
// - https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/rep.html
// - https://healeycodes.com/visualizing-chess-bitboards

enum {
    // white
    PC_WPAWN = 0,
    PC_WKNIGHT,
    PC_WBISHOP,
    PC_WROOK,
    PC_WQUEEN,
    PC_WKING,
    // black
    PC_BPAWN,
    PC_BKNIGHT,
    PC_BBISHOP,
    PC_BROOK,
    PC_BQUEEN,
    PC_BKING,
};

typedef uint8_t Piece;


// Chess Board represented with bitboards
// A8 is the most significant bit.
typedef struct {
    uint64_t white_pawns, black_pawns,
        white_knights, black_knights,
        white_bishops, black_bishop,
        white_rooks, black_rooks,
        white_queens, black_queens,
        white_kings, black_kings
    ;
} Board;

// Fill the board with the default chess position.
void board_setup(Board* b) {
    b->white_pawns = 0x00FF000000000000;    
    b->black_pawns = 0x000000000000FF00;

    b->white_knights = 0x0000000000000000;
    b->black_knights = 0x0000000000000000;
    b->white_bishops = 0x0000000000000000;
    b->black_bishop = 0x0000000000000000;
    b->white_rooks = 0x0000000000000000;
    b->black_rooks = 0x0000000000000000;
    b->white_queens = 0x0000000000000000;
    b->black_queens = 0x0000000000000000;
    b->white_kings = 0x0000000000000000;
    b->black_kings = 0x0000000000000000;
}


Piece board_get(const Board* b, unsigned x, unsigned y) {
    // TODO
}

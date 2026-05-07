#include <stdint.h>

// SOURCES:
// (chess bitboards used for inspiration)
// - https://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/rep.html
// - https://healeycodes.com/visualizing-chess-bitboards

// 8x8 board
typedef union {
    struct {
        uint32_t white;
        uint32_t black;
    };
    uint64_t pieces;
} Board;

// Fill the board with the default checkers position.
void board_setup(Board* b) {
}


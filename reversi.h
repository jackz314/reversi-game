#ifndef REVERSI_HPP
#define REVERSI_HPP

#include <iostream>
#include <vector>

/*******************************************************/
/* Add destructors, copy constructors, and assignment  */
/* operators to any class that requires one.           */
/*******************************************************/

/**
 * Square class models a location in Reversi
 * that can either be white, black, or free
 * using an enumerated type.
 */
struct Square {
    /**
     * Enumeration values act as named integer constants.
     * So Square::FREE acts as an integral constant 0,
     * Square::WHITE acts as an integral constant 1, and
     * Square::BLACK acts as an integral constant 2.
     */
    enum SquareValue { FREE = 0, WHITE, BLACK };

    // Data member
    SquareValue value_;

    /**
     * Constructor using default arguments to allow
     * no initial value that will default to FREE
     */
    Square(SquareValue value = FREE) : value_(value) { }

    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     * Assigns the internal SquareValue enumeration
     * to the provided input argument.
     * Note: The input is a SquareValue not a full Square
     */
    Square& operator=(SquareValue value);

    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     * Comparison operators to compare a Square
     * to a SquareValue enumeration value (i.e.
     *  Square::FREE, Square::WHITE, Square::BLACK)
     * Note: We are comparing a SquareValue not a full Square
     */
    bool operator==(SquareValue value) const;
    bool operator!=(SquareValue value) const;

    /**
     * Flip the current SquareValue from WHITE to BLACK or
     * BLACK to WHITE
     */
    void flip();
};

/*------------------- STUDENT TO WRITE -----------------*/
/**
 *  ostream operator for a Square object that outputs
 *    '-' for FREE, 'B' for BLACK, and 'W' for WHITE.
 *  Note: Since data members in the Square struct are public
 *    we do not need to make this a friend function.
 */
std::ostream& operator<<(std::ostream& out, const Square& square);

/**
 * Global helper to flip BLACK to WHITE and vice versa
 */
Square::SquareValue opposite_color(Square::SquareValue value);

/**
 * Board class models the 2D Reversi board
 * as an array of array of Squares. Uses
 * letters starting from 'a' to identify rows
 * and integers starting from 1 to identify
 * columns.
 */
class Board {
public:
    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     * Initializing constructor
     */
    Board(size_t s);
    Board(const Board &rhs);
    ~Board();

    Board& operator=(const Board &rhs);

    /** Access private size as read-only. */
    size_t dimension() const {
        return dimension_;
    }

    /**
     * To allow access to an internal square, we overload
     * the operator().
     * Given a board, b, this allows square access as `b('a',1);`
     * Will throw std::out_of_range if the row or column are
     * out of bounds.
     */
    Square& operator()(char row, size_t column);
    Square const& operator()(char row, size_t column) const;

    /**
     * Checks if the square value at the specifiedfli row and col are legal
     * locations and either opposite or the same as the square value
     * specified by `turn`.
     */
    bool is_legal_and_opposite_color(char row, size_t column, Square::SquareValue turn) const;
    bool is_legal_and_same_color(char row, size_t column, Square::SquareValue turn) const;

    /**
     *  Outputs the board
     */
    std::ostream& print(std::ostream& out) const;

    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     *  Outputs the board
     *  Should be a simple wrapper for Board::print().
     */
    friend std::ostream& operator<<(std::ostream& out, const Board& board);

    /**
     *  Count white and black pieces on the board, returns pair of (white, black) count
     */
    std::pair<int, int> count_pieces();

private:
    // Our actual board representation
    size_t dimension_;  // Dimension
    Square** squares_;  // 2D Square array

    /**
     *  Private helper to convert 'a' to 0, 'b' to 1, etc.
     */
    size_t row_to_index(char row) const {
        return (size_t)(row - 'a');
    }

    /**
     *  Private helper to check if a move can be made. (Only tests for out of bound values)
     */
    bool is_valid_location(char row, size_t column) const;
};


/**
 * Stores a board configuration (deep copy) and the current player's turn.
 */
struct Checkpoint {
    Board board_;
    Square::SquareValue turn_;

    /// Constructor
    Checkpoint(const Board& b, Square::SquareValue t);
};

/**
 * Reversi game class
 */
class Reversi {
public:
    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     * Constructs the board of a given side length (dimension)
     * Should set the middle 2x2 elements of the board to:
     *    BW
     *    WB
     */
    Reversi(size_t size);

    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     * Play the entire game.
     * Uses private helper functions to modularize the code.
     */
    void play();

private:
    /**
     * Prints the board and prompt for the next input/turn.
     */
    void prompt() const;

    /**
     * Prints the Win/Loss/Tie message using wcnt (white count)
     * and bcnt (black count)
     */
    void win_loss_tie_message(size_t white_count, size_t black_count);

    /**
     * Returns true if no legal move exists for the current player
     * given by current data member `turn_`
     */
    bool is_game_over() const;

    /**
     * Determines whether player given by `turn` can legally place at the
     * specified row and column. Will throw std::out_of_range if the
     * row or column are out of bounds.
     */
    bool is_legal_choice(char row, size_t column, Square::SquareValue turn) const;

    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     * Makes a checkpoint of the current board and player turn
     * and saves it in the history vector
     */
    void save_checkpoint();

    /*------------------- STUDENT TO WRITE -----------------*/
    /**
     * Overwrites the current board and player turn with the
     *  latest saved checkpoint. If no checkpoint is available
     *  simply return.
     */
    void undo();

    /**
     * Check if player has legal actions on the board
     */
    bool has_legal_choices(Square::SquareValue turn);

    void flip(char row_c, size_t col, Square::SquareValue turn);

    /* You may add other private helper functions */

private:
    // You do not need to add additional data members, but
    // may add them, provided they don't take the place of
    // or circumvent the purpose of the data members below.

    /// Current board state
    Board board_;

    /// Current player's ID (WHITE or BLACK)
    Square::SquareValue turn_;

    /// Saved checkpoints
    std::vector<Checkpoint> history_;
};

#endif

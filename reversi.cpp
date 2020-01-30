#include <vector>
#include <stdexcept>
#include <algorithm>    // std::max

#include "reversi.h"

using namespace std;

//utils

//0 to a, 1 to b, and so on
char index_to_row(size_t row)
{
    return (char) row + 'a';
}

//global helper one 'a' to 0, 'b' to 1, etc.
size_t row_to_index(char row) 
{
    return (size_t)(row - 'a');
}

Square::SquareValue flip_val(Square::SquareValue value)
{
    switch (value)
    {
    case Square::WHITE:
        return Square::BLACK;
    case Square::BLACK:
        return Square::WHITE;
    default:
        return Square::FREE;
    }
}

void Square::flip()
{
    switch (value_)
    {
    case WHITE:
        value_ = BLACK;
        break;
    case BLACK:
        value_ = WHITE;
        break;
    default:
        break;
    }
}

Square::SquareValue opposite_color(Square::SquareValue value)
{
    switch (value)
    {
    case Square::WHITE:
        return Square::BLACK;
    case Square::BLACK:
        return Square::WHITE;
    default:
        throw invalid_argument("Illegal square value");
    }
}

//impl
Square& Square::operator=(SquareValue value)
{
    value_ = value; 
    return *this;
}

bool Square::operator==(SquareValue value) const
{
    if(value_ == value) return true;
    else return false;
}

bool Square::operator!=(SquareValue value) const
{
    if(value_ != value) return true;
    else return false;
}

ostream& operator<<(ostream& out, const Square& square)
{
    switch (square.value_){
        case Square::WHITE:
            out << "W";
            break;
        case Square::BLACK:
            out << "B";
            break;
        case Square::FREE:
            out << "-";
            break;
        default:
            throw invalid_argument("Illegal square value");
    }
    return out;
}

Board::Board(size_t s)
{
    dimension_ = s;
    squares_ = new Square*[s];
    for (size_t i = 0; i < s; i++)
        squares_[i] = new Square[s];
}

Board::~Board()
{
    for (size_t i = 0; i < dimension_; i++)
        delete[] squares_[i];
    delete[] squares_;
}

//deep copy constructor
Board::Board(const Board &rhs)
{
    dimension_ = rhs.dimension();
    squares_ = new Square*[dimension_];
    for (size_t i = 0; i < dimension_; i++){
        squares_[i] = new Square[dimension_];
        for (size_t j = 0; j < dimension_; j++){
            squares_[i][j] = rhs(index_to_row(i),j+1);
        }
    }
}

//deep copy assignment
Board& Board::operator=(const Board &rhs)
{
    if(this == &rhs) return *this;
    if(squares_ != NULL){
        for (size_t i = 0; i < dimension_; i++)
            delete[] squares_[i];
        delete[] squares_;
    }
    dimension_ = rhs.dimension();
    squares_ = new Square*[dimension_];
    for (size_t i = 0; i < dimension_; i++){
        squares_[i] = new Square[dimension_];
        for (size_t j = 0; j < dimension_; j++){
            squares_[i][j] = rhs(index_to_row(i),j+1);
        }
    }
    return *this;
}

pair<int, int> Board::count_pieces()
{
    int w = 0, b = 0;
    for (size_t i = 0; i < dimension_; i++){//go through matrix to find legal block
        for (size_t j = 0; j < dimension_; j++){
            if(squares_[i][j] == Square::BLACK)
                b++;
            else if (squares_[i][j] == Square::WHITE)
                w++;
        }
    }
    return pair<int,int>(w,b);
}

ostream& operator<<(ostream& out, const Board& board)
{
    return board.print(out);
}

//end impl

Square& Board::operator()(char row, size_t column)
{
    if (!is_valid_location(row, column))
    {
        throw out_of_range("Bad row index");
    }
    size_t row_index = row_to_index(row);
    return squares_[row_index][column - 1];
}

Square const& Board::operator()(char row, size_t column) const
{
    if (!is_valid_location(row, column))
    {
        throw out_of_range("Bad row index");
    }
    size_t row_index = row_to_index(row);
    return squares_[row_index][column - 1];
}

bool Board::is_legal_and_opposite_color(
    char row, size_t column, Square::SquareValue turn) const
{
    if (is_valid_location(row, column))
    {
        size_t row_index = row_to_index(row);
        return squares_[row_index][column - 1] != Square::FREE && squares_[row_index][column - 1] != turn;
    }
    return false;
}

bool Board::is_legal_and_same_color(
    char row, size_t column, Square::SquareValue turn) const
{
    if (is_valid_location(row, column))
    {
        size_t row_index = row_to_index(row);
        return squares_[row_index][column - 1] == turn;
    }
    return false;
}

bool Board::is_valid_location(char row, size_t column) const
{
    size_t row_index = row_to_index(row);
    return row_index < dimension_ && column - 1 < dimension_;
}

Checkpoint::Checkpoint(const Board& b, Square::SquareValue turn) :
    board_(b),
    turn_(turn)
{}

ostream& Board::print(ostream& out) const
{
    out << "  ";
    for (size_t i = 1; i <= dimension_; i++)
    {
        if (i < 10)
        {
            out << " ";
        }
        else
        {
            out << (i / 10);
        }
    }
    out << endl;
    out << "  ";
    for (size_t i = 1; i <= dimension_; i++)
    {
        out << (i % 10);
    }
    out << endl;
    for (size_t i = 0; i < dimension_; i++)
    {
        out << (char)('a' + i) << ':';
        for (size_t k = 0; k < dimension_; k++)
        {
            out << squares_[i][k];
        }
        out << endl;
    }
    return out;
}

//impl 2

Reversi::Reversi(size_t size):
    board_(size)
{
    size_t mid = size/2-1;//left top location idx
    //starting pattern
    board_(index_to_row(mid), mid+1) = Square::BLACK;
    board_(index_to_row(mid+1), mid+1) = Square::WHITE;
    board_(index_to_row(mid), mid+1+1) = Square::WHITE;
    board_(index_to_row(mid+1), mid+1+1) = Square::BLACK;
    turn_ = Square::BLACK;//black start
}

bool Reversi::has_legal_choices(Square::SquareValue turn)
{
    if(turn==Square::FREE)
        return false;
    for (size_t i = 0; i < board_.dimension(); i++){//go through matrix to find legal block
        for (size_t j = 0; j < board_.dimension(); j++){
            if(is_legal_choice(index_to_row(i), j+1, turn))//is legal choice uses 1 based col
                return true;
        }
    }
    return false;
}

//here column is 1 based (used to be 0)
void Reversi::flip(char row, size_t col, Square::SquareValue turn)
{
    // Vectors for each cardinal direction
    const size_t direction_count = 8;
    const int direction_row[] =    {-1, -1,  0, +1, +1, +1,  0, -1};
    const int direction_column[] = { 0, -1, -1, -1,  0, +1, +1, +1};

    // Now check in each directions
    for (size_t d = 0; d < direction_count; d++)
    {
        // Where we're checking
        char cursor_row = row + direction_row[d];
        size_t cursor_column = col + direction_column[d];

        // Move towards the direction while we're on the opposite color
        bool found_opposite = false;
        while (board_.is_legal_and_opposite_color(cursor_row, cursor_column, turn))
        {
            found_opposite = true;
            cursor_row += direction_row[d];
            cursor_column += direction_column[d];
        }

        // Check if the next thing after is our color
        bool found_same = board_.is_legal_and_same_color(cursor_row, cursor_column, turn);

        // Flip
        if (found_opposite && found_same) {
            //reset cursor
            cursor_row = row + direction_row[d];
            cursor_column = col + direction_column[d];
            //start flipping
            while (board_.is_legal_and_opposite_color(cursor_row, cursor_column, turn))
            {
                board_(cursor_row, cursor_column) = turn;
                cursor_row += direction_row[d];
                cursor_column += direction_column[d];
            }
        }
    }

    // // size_t l_pos = -1, r_pos = -1, u_pos = -1, d_pos = -1, lu_pos = -1, ld_pos = -1, ru_pos = -1, rd_pos = -1;
    // int fst, lst;
    // //horizontal
    // //find flip range
    // for (size_t i = 0; i < board_.dimension(); i++)
    // {
    //     Square blk = board_(row_c, i+1);
    //     if(blk == turn){
    //         // if(col == i)//skip self
    //         //     continue;

    //         if(fst == -1)
    //             fst = i;
    //         lst = i;
    //     }else if(blk == Square::FREE){

    //     }
    // }
    // //flip
    // for (size_t i = fst; i <= lst; i++)
    //     board_(row_c, i+1) = turn;
    
    // fst = -1, lst = -1;

    // //vertical
    // //find flip range
    // for (size_t i = 0; i < board_.dimension(); i++)
    // {
    //     if(board_(index_to_row(i), col+1) == turn){
    //         // if(row == i)//skip self
    //         //     continue;
    //         if(fst == -1)
    //             fst = i;
    //         lst = i;
    //     }
    // }
    // //flip
    // for (size_t i = fst; i <= lst; i++)
    //     board_(index_to_row(i), col+1) = turn;
    
    // fst = -1, lst = -1;

    // //diagnal \ way
    // //find flip range
    // int diff = row - col, tmp_row=0, tmp_col=0;

    // if(diff > 0){
    //     tmp_row = diff;
    // }else{
    //     tmp_col = -diff;
    // }
    // for (size_t i = 0; i + max(tmp_row, tmp_col) < board_.dimension(); i++)
    // {
    //     if(board_(index_to_row(i+tmp_row), i+tmp_col+1) == turn){
    //         // if(row == i)//skip self
    //         //     continue;
    //         if(fst == -1)
    //             fst = i;
    //         lst = i;
    //     }
    // }
    // //flip
    
    // for (size_t i = fst; i <= lst; i++)
    //     board_(index_to_row(i+tmp_row), i+tmp_col+1) = turn;
    // fst = -1, lst = -1;

    // //diagnal / way
    // //find flip range
    // diff = board_.dimension()-1 - row - col;
    // tmp_row = 0, tmp_col = 0;
    // if(diff > 0){
    //     tmp_row = diff;
    // }else{
    //     tmp_col = -diff;
    // }
    // for (size_t i = 0; i + max(tmp_row, tmp_col) < board_.dimension(); i++)
    // {
    //     if(board_(index_to_row(board_.dimension()-1-(i+tmp_row)), i+tmp_col+1) == turn){
    //         // if(row == i)//skip self
    //         //     continue;
    //         if(fst == -1)
    //             fst = i;
    //         lst = i;
    //     }
    // }
    // //flip
    // for (size_t i = fst; i <= lst; i++)
    //     board_(index_to_row(board_.dimension()-1-(i+tmp_row)), i+tmp_col+1) = turn;
}

void Reversi::play()
{
    bool cont = true;
    char cmd;
    while (cont){
        //TODO: check if player can do anything
        if(!has_legal_choices(turn_))
            break;
        prompt();
        cin >> cmd;
        switch (cmd){
            case 'p':
                char row;
                size_t col;
                cin >> row >> col;//col is 1 based
                if(is_legal_choice(row,col,turn_)){
                    board_(row, col) = turn_;
                    //TODO: flip stuff
                    flip(row, col, turn_);//flip function's col is 1 based
                    turn_ = flip_val(turn_); //change player
                }//otherwise ignore it
                break;
            case 'c':
                save_checkpoint();
                break;
            case 'u':
                undo();
                break;
            default:
                cont = false;
                break;
        }
        if(is_game_over()) break;
    }
    int w_cnt, b_cnt;
    pair<int,int> wbcount = board_.count_pieces();
    w_cnt = wbcount.first;
    b_cnt = wbcount.second;
    win_loss_tie_message(w_cnt, b_cnt);
    //todo cleanup?
}

//Stack
void Reversi::save_checkpoint()
{
    history_.push_back(Checkpoint(board_, turn_));
}

void Reversi::undo()
{
    if(history_.empty())
        return;
    Checkpoint c(history_.back());
    board_ = c.board_;
    turn_ = c.turn_;
    history_.pop_back();
}

//end impl 2

void Reversi::prompt() const
{
    cout << board_ << endl;
    cout << (turn_ == Square::BLACK ? "B" : "W");
    cout << " - Enter 'p r/c', 'q', 'c', 'u':" << endl;
}

void Reversi::win_loss_tie_message(size_t white_count, size_t black_count)
{
    cout << board_ << endl;
    if (white_count > black_count)
    {
        cout << "W wins" << endl;
    }
    else if (white_count < black_count)
    {
        cout << "B wins" << endl;
    }
    else
    {
        cout << "Tie" << endl;
    }
    cout << "W=" << white_count << "/B=" << black_count << endl;
}

bool Reversi::is_legal_choice(char row, size_t column, Square::SquareValue turn) const
{
    // Vectors for each cardinal direction
    const size_t direction_count = 8;
    const int direction_row[] =    {-1, -1,  0, +1, +1, +1,  0, -1};
    const int direction_column[] = { 0, -1, -1, -1,  0, +1, +1, +1};

    // Make sure location is free
    if (board_(row, column) != Square::FREE)
    {
        return false;
    }

    // Now check in each directions
    for (size_t d = 0; d < direction_count; d++)
    {
        // Where we're checking
        char cursor_row = row + direction_row[d];
        size_t cursor_column = column + direction_column[d];

        // Move towards the direction while we're on the opposite color
        bool found_opposite = false;
        while (board_.is_legal_and_opposite_color(cursor_row, cursor_column, turn_))
        {
            found_opposite = true;
            cursor_row += direction_row[d];
            cursor_column += direction_column[d];
        }

        // Check if the next thing after is our color
        bool found_same = board_.is_legal_and_same_color(cursor_row, cursor_column, turn_);

        // If this direction is valid, the move is valid, so short circuit and return
        if (found_opposite && found_same) {
            return true;
        }
    }
    return false;
}

bool Reversi::is_game_over() const
{
    for (unsigned char row = 'a'; row < 'a' + board_.dimension(); row++)
    {
        for (size_t column = 1; column <= board_.dimension(); column++)
        {
            if (is_legal_choice(row, column, turn_))
            {
                return false;
            }
        }
    }
    return true;
}




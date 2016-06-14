/* 
 * File:   chessBoard.h
 * Author: Prof. Ari Trachtenberg
 *
 * Created on March 18, 2015, 9:35 PM
 */

#ifndef CHESSBOARD_H
#define	CHESSBOARD_H

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class chessBoard {
public:
    // CONSTANTS
    const static int minRow = 1; // the minimum row number
    const static int maxRow = 8; // the maximum row number
    const static char minCol = 'a'; // the minimum column letter
    const static char maxCol = 'h'; // the maximum column letter

    // ENUMERATIONS

    /**
     A piece on the board.
     */
    enum pieceType {
        King, Queen, Rook, Bishop, Knight, Pawn
    };

    enum colorType {
        White, Black
    };

    // AUXILLIARY CLASSES

    /**
     A square on the chess board
     */
    class square {
    public:

        square(char col1='a', int row1=1) : col(col1), row(row1) {
        }

         /** Copies a square. */
        square operator=(const square& other) {
            col = other.col;
            row = other.row;
            return *this;
        }

        /** Returns a human-readable version of the square. */
        string print() const {
            return string(1, col) + string(1, '0' + row);
        }
        /** Checks two squares for equality. */
        bool operator==(const square& other) { return col==other.col && row==other.row; }
        char col; /** a character representing the column of the piece. */
        int row;  /** number representing the row of the square (starts at 1 for legacy reasons) */
    };

    /**
     A chess piece.
     */
    class piece {
    public:

        piece(pieceType type1, colorType color1, char col, int row) : type(type1), color(color1), loc(col, row) {
        }

        char print() const { /** prints a character representing the piece; upper-case for white, lower-case for black. */
            switch (type) {
                case King: if (color == White) return 'K';
                    else return 'k';
                case Queen: if (color == White) return 'Q';
                    else return 'q';
                case Bishop: if (color == White) return 'B';
                    else return 'b';
                case Knight: if (color == White) return 'N';
                    else return 'n';
                case Rook: if (color == White) return 'R';
                    else return 'r';
                case Pawn: if (color == White) return 'P';
                    else return 'p';
            }
        }
        colorType color; /** The piece's color. */
        pieceType type; /** The type of piece. */
        square loc; /** The location of the piece. */
    };

    /**
     A move on the chess board.
     */
    class move {
    public:

        /**
         * Represents a move between two squares on the board.
         * @param col1 column of "from" square
         * @param row1 row of "from" square
         * @param col2 column of "to" square
         * @param row2 row of "to" square
         * @requires minRow <= col1,col2 <= maxRow
         * @requires minCol <= row1,row2 <= maxCol
         */
        move(char col1='a', int row1=0, char col2='a', int row2=0) : from(col1, row1), to(col2, row2) {}
        move(square s1, char col2, int row2) : from(s1.col, s1.row), to(col2, row2) {}
        move(square s1, square s2) : from(s1.col, s1.row), to(s2.col, s2.row) {}

        /**
         * Equality check between two moves.
         */
        bool operator==(const move& other) {return from==other.from && to==other.to; }
        
        /**
         * @return A human-readable version of this move.
         */
        string print() const {
            return from.print()+" - " + to.print();
        }
        square from, to; /** move from <from> to <to> */
    };

    // ... EXCEPTIONS

    class NoPieceException : public exception {};
    class InvalidMoveException : public exception {
    public:
        InvalidMoveException(const move OffendingMove) : badMove(OffendingMove) {}
        move badMove; /** the move that caused the exception */
    };
    // CONSTRUCTORS
    /**
     * Creates a new, empty chess board.
     */
    chessBoard();

    /**
     * Copies an existing chessBoard
     * @param orig
     */
    chessBoard(const chessBoard& orig) {
        pieceList = orig.pieceList;
        moveVect = orig.moveVect;
        toMoveColor=orig.toMoveColor;
    }

    // ACTION
    /**
     * Makes a move on the board.
     * @throw InvolidMoveException if the move is not valid
     */
    void makeMove(const move theMove);
    void makeMove(const square from, const square to) { makeMove(move(from,to)); }
    void makeMove(const char from_col, int from_row, char to_col, int to_row) { makeMove(square(from_col,from_row),square(to_col,to_row)); }
    
    // QUERY
    /**
     * @param row The row on the board.
     * @param col The column on the board.
     * @return a piece at board position (col,row)
     * @throws NoPieceException if no piece exists at this location
     */
    const piece atSquare(char col, int row) const { return atSquare(square(col,row)); }
    const piece atSquare(square sq) const;
    
    /**
     * @param row
     * @param col
     * @return true iff the square at (row,col) is empty
     */
    bool emptySquareQ(char col, int row);
    bool emptySquareQ(square sq) { return emptySquareQ(sq.col, sq.row); }
    /**
     * @return The number of moves available from the current position
     * 
     */
    int numMoves() {
        return moveVect.size();
    }

    /**
     * @param nn
     * @return The nn-th move available from the current position.
     */
    const move nthMove(int nn);
    
    /**
     * @return If the game, in the current position, is over.
     */
    bool gameOverQ() const;
    
    
    // DISPLAY
    /**
     * Shows the current configuration of the board in a human-readable way.
     * @return A string representing the current board position.
     */
    string print() const;

     /**
     * @return A human-readable version of the current move list
     */
    string printMoves();
 
private:
    list<piece> pieceList; /** The pieces on the board. */
    vector<move> moveVect; /** A vector of moves available from the current board position. */
    colorType toMoveColor; /** The color to make the next move. */

    /**
     * Populates the moveVect vector for the current board with color <toMoveColor> to move
     */
    void createMoveVect();

    /**
     * Adds move mv to the moveVect iff
     * 1.  the coordinates within mv are all on the board
     * 2.  there is no piece of color <color> at the "to" square of the move
     * @return true iff a valid move was produced.
     * @param color The color of the side making the move.
     * @param mv The move being considered
     */
    bool addMove(colorType color, move mv);
    
    /**
     * Adds a move from *piecePtr to the <dest> square.
     */
    bool addMove(list<piece>::const_iterator piecePtr, square dest) { return addMove(piecePtr->color,move(piecePtr->loc,dest)); }
    bool addMove(list<piece>::const_iterator piecePtr, char col, int row) { return addMove(piecePtr,square(col,row)); }
    bool addMove(colorType color, square start, char col, int row) { return addMove(color,move(start,col,row)); }
    /**
     * Repeatedly addMoves from start to start+colInc,start+rowInc until a non-empty square is found
     */
    void repeatMove(list<piece>::const_iterator piecePtr, int colInc, int rowInc);
    
    /**
     * A private, maleable version of atSquare
     * @param sq The location on the board.
     * @return An iterator pointing to the piece at the given square.
     */
    list<piece>::iterator atSquareInternal(square sq);
};

#endif	/* CHESSBOARD_H */
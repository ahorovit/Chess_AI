/* 
 * File:   chessBoard.cpp
 * Author: Prof. Ari Trachtenberg
 * 
 * Created on March 18, 2015, 9:35 PM
 */

#include "chessBoard.h"

chessBoard::chessBoard() {
    // Set up the white pieces
    pieceList.push_back(piece(King, White, 'e', minRow));
    pieceList.push_back(piece(Queen, White, 'd', minRow));
    pieceList.push_back(piece(Bishop, White, 'c', minRow));
    pieceList.push_back(piece(Bishop, White, 'f', minRow));
    pieceList.push_back(piece(Knight, White, 'b', minRow));
    pieceList.push_back(piece(Knight, White, 'g', minRow));
    pieceList.push_back(piece(Rook, White, 'a', minRow));
    pieceList.push_back(piece(Rook, White, 'h', minRow));
    for (char col = minCol; col <= maxCol; col++)
        pieceList.push_back(piece(Pawn, White, col, 2));

    // Set up the black pieces
    pieceList.push_back(piece(King, Black, 'e', maxRow));
    pieceList.push_back(piece(Queen, Black, 'd', maxRow));
    pieceList.push_back(piece(Bishop, Black, 'c', maxRow));
    pieceList.push_back(piece(Bishop, Black, 'f', maxRow));
    pieceList.push_back(piece(Knight, Black, 'b', maxRow));
    pieceList.push_back(piece(Knight, Black, 'g', maxRow));
    pieceList.push_back(piece(Rook, Black, 'a', maxRow));
    pieceList.push_back(piece(Rook, Black, 'h', maxRow));
    for (char col = minCol; col <= maxCol; col++)
        pieceList.push_back(piece(Pawn, Black, col, 7));

    toMoveColor=White;
    createMoveVect();
}

const chessBoard::piece chessBoard::atSquare(square sq) const {
    list<chessBoard::piece>::const_iterator it;
    for (it = pieceList.begin(); it != pieceList.end(); it++) {
        if (it->loc.col == sq.col && it->loc.row == sq.row)
            return *it;
    }
    // i.e. no piece was found here
    throw NoPieceException();
}

list<chessBoard::piece>::iterator chessBoard::atSquareInternal(square sq) {
    list<chessBoard::piece>::iterator it;
    for (it = pieceList.begin(); it != pieceList.end(); it++) {
        if (it->loc.col == sq.col && it->loc.row == sq.row)
            return it;
    }
    // i.e. no piece was found here
    throw NoPieceException();
}

string chessBoard::print() const {
    string result;

    for (int row = minRow - 1; row <= maxRow; row++) {
        for (char col = minCol - 1; col <= maxCol; col++)
            if (row == 0)
                if (col == '`')
                    result += "`  | ";
                else
                    result += " " + string(1, col) + " "; // print the column heading
            else
                if (col == '`')
                result += " " + string(1, '0' + row) + " | "; // print the row number
            else {
                try {
                    result += " " + string(1, atSquare(col, row).print()) + " ";
                } catch (NoPieceException e) { // i.e. no piece here
                    result += " - ";
                }
            }
        if (row == 0)
            result += "\n____________________________\n";
        else
            result += '\n';
    }
    return result;
}

string chessBoard::printMoves() {
    string result = "";
    for (int ii = 0; ii < numMoves(); ii++)
        result += nthMove(ii).print() + "\n";
    return result;
}

bool chessBoard::emptySquareQ(char col, int row) {
    try {
        atSquare(col, row);
        return false;
    } catch (NoPieceException e) {
        return true;
    }
}

bool chessBoard::addMove(colorType color, move mv) {
    if (mv.from.col < minCol || mv.from.col > maxCol || mv.from.row < minRow || mv.from.row > maxRow // move is within the board?
            || mv.to.col < minCol || mv.to.col > maxCol || mv.to.row < minRow || mv.to.row > maxRow)
        return false; // case 0.  Invalid move - leaves the chess board.
    try {
        piece dest = atSquare(mv.to);
        if (dest.color == color) // Case 1. Invalid move - trying to move on top of another piece of the same color.
            return false;
        else { // Case 2.  Valid move - capture an enemy piece.
            moveVect.push_back(mv);
            return true;
        }
    } catch (NoPieceException e) { // Case 3.  Valid move - no capture.
        moveVect.push_back(mv);
        return true;
    }
}

void chessBoard::repeatMove(list<piece>::const_iterator piecePtr, int colInc, int rowInc) {
    char col;
    int row;
    for (col = piecePtr->loc.col + colInc, row = piecePtr->loc.row + rowInc;
            addMove(piecePtr->color, piecePtr->loc, col, row) && emptySquareQ(col, row);
            col += colInc, row += rowInc)
        ;
}

void chessBoard::createMoveVect() {
    moveVect.clear(); // clear any existing moves

    list<chessBoard::piece>::const_iterator it;
    for (it = pieceList.begin(); it != pieceList.end(); it++)
        if (it->color != toMoveColor) // only consider pieces of the color that is slated to move
            continue;
        else switch (it->type) {
                case Pawn: // a pawn moves one square ahead, unless it is blocked by a piece or falls off the board
                    if (it->color == White) {
                        if (addMove(it, it->loc.col, it->loc.row + 1) && !emptySquareQ(it->loc.col, it->loc.row + 1))
                            moveVect.pop_back(); // pawns can only move, not capture in this way
                        if (addMove(it, it->loc.col + 1, it->loc.row + 1) && emptySquareQ(it->loc.col + 1, it->loc.row + 1))
                            moveVect.pop_back(); // pawns can only capture, not move in this way
                        if (addMove(it, it->loc.col - 1, it->loc.row + 1) && emptySquareQ(it->loc.col - 1, it->loc.row + 1))
                            moveVect.pop_back(); // pawns can only capture, not move in this way                       
                    }
                    if (it->color == Black) {
                        if (addMove(it, it->loc.col, it->loc.row - 1) && !emptySquareQ(it->loc.col, it->loc.row - 1))
                            moveVect.pop_back(); // pawns can only move, not capture in this way
                        if (addMove(it, it->loc.col + 1, it->loc.row - 1) && emptySquareQ(it->loc.col + 1, it->loc.row - 1))
                            moveVect.pop_back(); // pawns can only capture, not move in this way
                        if (addMove(it, it->loc.col - 1, it->loc.row - 1) && emptySquareQ(it->loc.col - 1, it->loc.row - 1))
                            moveVect.pop_back(); // pawns can only capture, not move in this way                       
                    }
                    break;
            case Knight: // a knight makes two steps in one direction and one step in a perpendicular direction
                addMove(it, it->loc.col + 2, it->loc.row + 1);
                addMove(it, it->loc.col + 2, it->loc.row - 1);
                addMove(it, it->loc.col - 2, it->loc.row + 1);
                addMove(it, it->loc.col - 2, it->loc.row - 1);
                addMove(it, it->loc.col + 1, it->loc.row + 2);
                addMove(it, it->loc.col + 1, it->loc.row - 2);
                addMove(it, it->loc.col - 1, it->loc.row + 2);
                addMove(it, it->loc.col - 1, it->loc.row - 2);
                break;
            case Bishop:
                repeatMove(it, 1, 1);
                repeatMove(it, -1, 1);
                repeatMove(it, 1, -1);
                repeatMove(it, -1, -1);
                break;
            case Rook:
                repeatMove(it, 0, 1);
                repeatMove(it, 0, -1);
                repeatMove(it, 1, 0);
                repeatMove(it, -1, 0);
                break;
            case Queen:
                                repeatMove(it, 1, 1);
                repeatMove(it, -1, 1);
                repeatMove(it, 1, -1);
                repeatMove(it, -1, -1);
                                repeatMove(it, 0, 1);
                repeatMove(it, 0, -1);
                repeatMove(it, 1, 0);
                repeatMove(it, -1, 0);
                break;
            case King:
                addMove(it, it->loc.col + 1, it->loc.row + 1);
                addMove(it, it->loc.col + 1, it->loc.row + 0);
                addMove(it, it->loc.col + 1, it->loc.row - 1);
                addMove(it, it->loc.col + 0, it->loc.row + 1);
                addMove(it, it->loc.col + 0, it->loc.row - 1);
                addMove(it, it->loc.col - 1, it->loc.row + 1);
                addMove(it, it->loc.col - 1, it->loc.row + 0);
                addMove(it, it->loc.col - 1, it->loc.row - 1);
                break;
        }
}

const chessBoard::move chessBoard::nthMove(int nn) {
    return moveVect[nn];
}

void chessBoard::makeMove(const move theMove) {
    // Check that the move is valid
    if (find(moveVect.begin(),moveVect.end(),theMove) == moveVect.end())
        throw InvalidMoveException(theMove);
    
    // Effect the move
    // ... remove a captured piece, if the square is not empty
    if (!emptySquareQ(theMove.to))
        pieceList.erase(atSquareInternal(theMove.to));
    // ... change the location of the moving piece
    list<chessBoard::piece>::iterator thePiece = atSquareInternal(theMove.from);
    thePiece->loc = theMove.to;
    
    // ... recalculate the move vector
    toMoveColor = (toMoveColor==White?Black:White); // switch color to move
    createMoveVect();
}

bool chessBoard::gameOverQ() const {
    // case 1.  no moves left
    if (moveVect.size()==0)
        return true;
    
    // case 2.  check for Kings
    bool whiteKing=false;
    bool blackKing=false;
            
    list<chessBoard::piece>::const_iterator it;
    for (it = pieceList.begin(); it != pieceList.end(); it++)
        if (it->type == King) {
            if (it->color==White)
            whiteKing=true;
            else
            blackKing=true;
        }
        return (!whiteKing || !blackKing);
}
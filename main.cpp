/* 
 * File:   main.cpp
 * Author: arit
 *
 * Created on March 18, 2015, 6:24 PM
 */

#include <cstdlib>
#include <iostream>
#include "chessBoard.h"
#include "chessPlayer.h"
#include "myPlayer.h"

using namespace std;
/*
 * Usage:  chess with no arguments makes the player white.
 *         chess with any arguments make the player black.
 */
int main(int argc, char** argv) {
    // Declare and initialize variables
    chessBoard gameBoard; // the official game board
    
    // ... player is White, unless an argument is passed
    chessBoard::colorType playerColor=chessBoard::White;
    if (argc>1)
        playerColor=chessBoard::Black;
    chessPlayer *player = new myPlayer(playerColor); // the computer player
    chessBoard::square from, to;
    
    cout << "Welcome to Homework Six simplified chess." << endl
            << "  Please enter your moves in the format 'e2-e3'." << endl
            << "  You are playing " << (playerColor==chessBoard::White?"White":"Black") << "." << endl << endl;
    
    // Make chess moves until the game ends.
    chessBoard::colorType toMove = chessBoard::White;  // the color to move
    try {
        for (;
                !gameBoard.gameOverQ();
                toMove = (toMove == chessBoard::White ? chessBoard::Black : chessBoard::White)) {

            if (playerColor == toMove) { // get a move
                char spacer;
                cout << gameBoard.print() << endl; // show the game board
                // cout << "Valid moves are: " << endl << gameBoard.printMoves() << endl;
                cout << "Please enter your move: ";
                cin >> from.col >> from.row >> spacer >> to.col >> to.row;
                chessBoard::move playerMove = chessBoard::move(from, to);
                try {
		  gameBoard.makeMove(playerMove); // enact the move
		  player->enemyMove(playerMove); // record the move with the computer player
                } 
		
		catch (chessBoard::InvalidMoveException e) {
		  cout << e.badMove.print() << " is an invalid move!" << endl;
		  cout << "Valid moves were: " << endl << gameBoard.printMoves() << endl;

		  // give another chance (skip enemy turn)
		  toMove = (toMove == chessBoard::White ? chessBoard::Black : chessBoard::White);
		  continue;		  
		}
            } else {
                chessBoard::move computerMove;
                try {
                    computerMove = player->getMove();
                    } catch (exception e) { cout << "EXCEPTION2" << endl;} // we ignore the player's exceptions, but you shouldn't     
                    cout << "I move: " << computerMove.print() << endl;
                    gameBoard.makeMove(computerMove); // make the computer's move
            }
                
        }
    } 	
    catch (exception e) { cout << "EXCEPTION1" << endl;} // we ignore the player's exceptions, but you shouldn't


    // We got here because the game is over ... see who wins
    cout << endl << "Game Over:  " << (toMove==chessBoard::White?"Black":"White") << " wins!" << endl;
    return toMove;
}


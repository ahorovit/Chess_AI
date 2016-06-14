/* 
 * File:   myPlayer.h
 * Author: Adin Horovitz
 * 
 * 
 */

#ifndef MYPLAYER_H
#define	MYPLAYER_H
#include <cstdlib>
#include "chessBoard.h"
#include "chessPlayer.h"

class piece;
class move;
//enum pieceType;


class myPlayer : public chessPlayer {
 
 public:
 myPlayer(chessBoard::colorType col) : chessPlayer(col) { 
    srand(time(NULL)); 
  }
  
  virtual void enemyMove(chessBoard::move theMove) {
    myBoard.makeMove(theMove); 
  }
  

  virtual chessBoard::move getMove();
  
  int pcPoints(chessBoard::piece target){  // fetch number of pts for piece
    switch (target.print()) {
    case 'K': case 'k': return 100;
    case 'Q': case 'q': return 9;
    case 'B': case 'b': return 3;
    case 'N': case 'n': return 3;
    case 'R': case 'r': return 5;
    case 'P': case 'p': return 1;
    }
  }

  int pcWeight(chessBoard::piece target){  // fetch movement weight of pc
    switch (target.print()) {
    case 'K': case 'k': return 1;
    case 'Q': case 'q': return 3;
    case 'B': case 'b': return 2;
    case 'N': case 'n': return 3;
    case 'R': case 'r': return 4;
    case 'P': case 'p': return 1;
    }
  }
  
 private:
  chessBoard myBoard; // internal version of the current board
  
  // A.I. here --> try to find best move, pick randomly otherwise
  chessBoard::move findBest(chessBoard& present, int &score, int &mvCount, bool lookAhead);
  chessBoard testBoard;

};


class NoBestException : public exception {};


#endif	/* MYPLAYER_H */


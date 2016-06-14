/* 
 * File:   chessPlayer.h
 * Author: Ari Trachtenberg
 *
 * Created on March 25, 2015, 3:58 PM
 */

#ifndef CHESSPLAYER_H
#define	CHESSPLAYER_H
#include "chessBoard.h"

class chessPlayer {
public:

    /**
     * Starts a chess player from a new board.
     * @param myColor The color that this player is playing
     */
    chessPlayer(chessBoard::colorType col) : myColor(col) {}
    
    /**
     * Record an enemy's move.
     * @param theMove made by the enemy
     */
    virtual void enemyMove(chessBoard::move theMove)=0;
    
    /**
     * Compute my move
     * @return the computed move.
     */
    virtual chessBoard::move getMove()=0;

private:
    chessPlayer() {}  // i.e. don't use this constructor
protected:
    chessBoard::colorType myColor;
};

#endif	/* CHESSPLAYER_H */


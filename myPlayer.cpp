


/*
**  -------------------Algorithm to find bestMove in simplified chess-------------------
**                            Adin Horovitz -- EC330 HW6 -- 4/1/15
**
**  Loops through all valid moves and calculates: 
**  
**  Score = pointsGained (from capture) - pointsLost (from enemy response)
**  
**  --> findBest() calls itself with new arguments to look ahead to the enemy's move.
**  --> When looking a move ahead, does not call findBest again --> returns less sophisticated
**     best move (only based on capture), which is passed back along with ptsLost to first findBest() call.
**  --> back in present state of board, Score is calculated and any ties are stored for tie breaking.
**
**  Ties for bestMove are won by whichever move results in the MOST DEVELOPMENT....
**  --> at the end of the lookAhead call to findBest(), testBoard is advanced once more (ie original
**     player's NEXT turn) and the number of moves is calculated and passed back to calling scope.
**  --> Thus, moves that open up lanes of movement are favored!
**
**  After all moves have been analyzed, best move is selected:
**  Priority 1: move that generates the highest Score
**  Priority 2: among moves tied for highest score, move resulting in most moves NEXT turn
**
*/



#include <iostream>
#include "myPlayer.h"
#include <fstream> // needed for printf() debugging output (breaks code submission)


// Interface function b/w provided score and AI code
chessBoard::move myPlayer::getMove() {
  
  chessBoard::move theMove;        // container for move to execute

  int score=0, mvCount = 0;        // placeholder ints --> these are used in "recurs-ish" call
  theMove = findBest(myBoard, score, mvCount, true);   
  
  myBoard.makeMove(theMove); 
  
  return theMove;
}


// AI code
chessBoard::move myPlayer::findBest(chessBoard& present, 
				    int &score, int &mvCount, bool lookAhead){

  chessBoard::move bestMove;                       // Container for move deemed the best

  vector< pair<chessBoard::move,int> > tiedMoves;  // stores tied moves and the nextMvCount

  int maxPts = -100;                               // initial maxPts --> overwritten by valid move
  int ptsLost = 0, nextMvCt = 0, pcVal = 0;        // ints passed by reference in lookAhead mode

  // Loop through all legal moves
  for(int ii=0; ii<present.numMoves(); ii++){
    
    chessBoard::move theMove = present.nthMove(ii);           // move being tested

    // Calculate points gained (by capture) --> requires move.to to be occupied by enemy
    score = 0;
    if(!present.emptySquareQ(theMove.to)){
      chessBoard::piece dest = present.atSquare(theMove.to);  // enemy in square
      pcVal = pcPoints(dest);                                 // how many pts is it worth?
      score += pcVal;
    }
    else
      pcVal = 0;  // in case previous move's pcVal is still stored

    // only project ahead one turn (for now...)
    if(lookAhead){

      testBoard = myBoard;                                     // Reset testBoard to current state
      testBoard.makeMove(theMove);                      
      
      // See what enemy would respond with (based ONLY on points gained from takeback) --> does not look ahead again
      // Because ptsLost and nextMvCt are passed BY REFERENCE, these values are set in recursive call.
      chessBoard::move enemyMv = findBest(testBoard, ptsLost, nextMvCt, false);             
      
      score -= ptsLost;  // Subtract takeback points from score

      // if new maxPts found, tiedMoves list is invalid
      if(score > maxPts)
	tiedMoves.clear();  
      
      // maintain list of tiedMoves and their nextMvCt
      if(score >= maxPts){
	tiedMoves.push_back(pair<chessBoard::move,int>(theMove,nextMvCt));
	bestMove = theMove;
	maxPts = score;	
	
	printf("%s......",theMove.print().c_str());       // Debugging output
        printf("Benefit: %d.....", pcVal);    
	printf("Cost: -%d  (%s) ..... nextMvCount = %d\n", ptsLost, enemyMv.print().c_str(), nextMvCt); 
	
      }
    }

    //if NOT looking ahead (recursive call), bestMove is simpler (retains score after pcVal was calculated)
    else{                       
      if(score >= maxPts){
	bestMove = theMove;
	maxPts = score;
      }
    }
  }
  
  score = maxPts;  // THIS IS NECESSARY!!! --> makes ptsLost by takeback available in calling scope

  // If not looking ahead, make best move for OTHER player and count numMoves (saved in tiedMoves)
  //  --> this is executed in recursive call, and used back in first findBest() call.
  if(!lookAhead){
    


    present.makeMove(bestMove);    // NOTE: present is ACTUALLY the enemy's next move in this scope
                                   //   --> so when we advance the board, we are looking at original players NEXT move
    mvCount = present.numMoves();  //mvCount was PBR, so it's available in calling scope
    
    chessBoard::move thisMv;
    int weightedMvCt = 0;
    bool vipFlag = false;

    for(int ii=0; ii<mvCount; ii++){
      
      thisMv = present.nthMove(ii);
      chessBoard::piece thisPc = present.atSquare(thisMv.from);

      weightedMvCt += pcWeight(thisPc);                               // moves by certain pcs are worth more
     
      if(!present.emptySquareQ(thisMv.to)){
	chessBoard::piece target = present.atSquare(thisMv.to);
      
	char pcID = target.print();
	if(pcID == 'K' || pcID == 'k')// || pcID == 'Q' || pcID == 'q' ) // strongly favor K/Q targets
	vipFlag = true;
      }
    }

    if(vipFlag)
      weightedMvCt += 1000;

    mvCount = weightedMvCt;
  }
  
  // If there were tied moves, pick the one with max development (nextMvCt * pcWeight)
  if(tiedMoves.size() > 0){  

    int maxMvs = 0;
    for(int jj = 0; jj<tiedMoves.size(); jj++){

      if(tiedMoves[jj].second > maxMvs){
	bestMove = tiedMoves[jj].first;
	maxMvs = tiedMoves[jj].second;
      }
    }
  }
  return bestMove;

}

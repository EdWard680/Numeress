#ifndef _AI_H_
#define _AI_H_

#include "boardTree.h"

/*
struct Node
{
  Board b;
  int depth;
  Piece * p;
  Cell * c;
  EXPECTED_VALUE prev;
};

struct ABNode
{
  Board b;
  int depth;
  Piece * p;
  Cell * c;
  EXPECTED_VALUE alpha;
  EXPECTED_VALUE beta;
};
*/

class AI
{
  Board *game;
  const SIDE_t side;
  
public:
  const EXPECTED_VALUE evaluate(const Board &b);
  const EXPECTED_VALUE quiEvaluate(const Board &b) {return b.whosTurn() == side? (evaluate(b)):(-evaluate(b));};
  const SIDE_t getSide() const {return side;};
private:
  //const EXPECTED_VALUE findBestMove(BoardNode &n);  //uses werid average thing I developed
  //const EXPECTED_VALUE quiesce(BoardNode &n);
  const SEARCH_RETURN alphaBeta(Board &board, const EXPECTED_VALUE alpha, const EXPECTED_VALUE beta, const int depth);
  
public:
  const Move getBestMove(const int depth);
  
public:
  AI(Board * const b, const SIDE_t s): side(s), game(b) {};
};

#endif
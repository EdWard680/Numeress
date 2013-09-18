#ifndef _AI_H_
#define _AI_H_

#include "boardTree.h"


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


class AI
{
  const Board * board;
  const SIDE_t side;
  
public:
  const EXPECTED_VALUE evaluate(const Board &b);
  const EXPECTED_VALUE quiEvaluate(const Board &b) {return b.whosTurn() == side? (evaluate(b)):(-evaluate(b));};
  const SIDE_t getSide() const {return side;};
private:
  const EXPECTED_VALUE findBestMove(Node &n);  //uses werid average thing I developed
  const EXPECTED_VALUE quiesce(ABNode &n);
  const EXPECTED_VALUE alphaBeta(ABNode &n);
  
public:
  const ABNode getBestMove(const Uint8 depth);
  
public:
  AI(const Board * const b, const SIDE_t s): side(s), board(b) {};
};






#endif
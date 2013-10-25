#ifndef BOARDTREE_H_
#define BOARDTREE_H_

#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "board.h"

typedef double EXPECTED_VALUE;

class AI;
class BoardNode;

//class Node;

class Move
{
  mutable Board *b;
  mutable Piece *p;
  mutable Cell *c;
  mutable Piece *victim;
  mutable short pFlag;
  mutable short vicFlag;
  
public:
  Piece * const getPiece() const {return p;};
  Cell * const getCell() const {return c;};
  Board * const getBoard() const {return b;};
  
  Piece * const getPiece(const Board &other) const {return other.getPiece(p->getValue(), p->getSide());};
  Cell * const getCell(const Board &other) const {return other.getCell(b==NULL? Point(0,0):b->getPoint(c));};
  
  Piece * const setPiece(Piece * const other) {return p = other;};
  Cell * const setCell(Cell * const other) {return c = other;};
  Board * const setBoard(Board * const other) {return b = other;};
  
  Piece * const setPiece(Piece * const othPiece, const Board &othBoard) {return p = (b==NULL? NULL:b->getPiece(othPiece->getValue(), othPiece->getValue()));};
  Cell * const setCell(Cell * const othCell, const Board &othBoard) {return c = (b==NULL? NULL:b->getCell(othBoard.getPoint(othCell)));};
  void changeBoard(Board& other) const ;
  
  const bool doMove() const;
  const bool undoMove() const;
  
  const bool operator== (const Move& other) const {return p==other.p && c==other.c;};
  const bool operator!= (const Move& other) const {return !(*this == other);};
  
  Move(Piece *pInit=NULL, Cell *cInit=NULL, Board *bInit=NULL): p(pInit), victim(NULL), pFlag(-1), vicFlag(-1), c(cInit), b(bInit) {};
  Move(const Move& other): p(other.p), c(other.c), b(other.b), victim(NULL), pFlag(-1), vicFlag(-1) {};
  Move(const Move& other, Board * const bInit): Move((bInit? other.getPiece(*bInit):NULL), (bInit? other.getCell(*bInit):NULL), bInit) {};
};

struct SEARCH_RETURN
{
  EXPECTED_VALUE val;
  Move bestMove;
  SEARCH_RETURN(const EXPECTED_VALUE v, const Move m): val(v), bestMove(m) {};
};

namespace std
{
  template<>
  struct hash<Move>
  {
    size_t operator() (const Move& m) const
    {
      return ((hash<Piece *>()(m.getPiece())
               ^ (hash<Cell *>()(m.getCell()) << 1)));
    };
  };
  /*
  template<>
  struct pair<const Move, BoardNode*>
  {
    pair(const Move& a, const BoardNode* b);
  };*/
}

class BoardNode
{
public:
  typedef unordered_map<Move, BoardNode*> CHILD_LIST;
  
private:
  Board b;
  AI *pov;  //uses to evaluate
  EXPECTED_VALUE val;
  
public:
  // const member functions
  const Board &getBoard() const {return b;};
  const EXPECTED_VALUE getValue() const {return val;};
  
public:
  // non_const member functions
  void setBoard(const Board& other) {b = other;};
  // Builds game tree to depth. Returns negative number on failure. If passed negative number, IT WILL ATTEMPT TO BUILD ENTIRE GAME TREE
  const int generateChildren(const int depth); // This is a fancy term for "have sex," kids
  const bool doMove(const Move& m);
  
public:
  // constructors and destructors
  BoardNode(): val(0.0), pov(NULL) {children = new CHILD_LIST;};
  BoardNode(const Board& bInit, AI * const ai);
  ~BoardNode();
private:
  CHILD_LIST *children; //mah children, must be allocated.
public:
  CHILD_LIST::const_iterator firstIter() const {return children->cbegin();};
  CHILD_LIST::const_iterator endIter() const {return children->cend();};
};


#endif
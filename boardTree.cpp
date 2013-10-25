#include "AI.h"

/*
namespace std
{
  pair<Move, BoardNode *>::pair(const Move &a, BoardNode * const b): first(a), second(b)
  {
    a.setBoard(&b->getBoard());
    a.doMove();
  }
}
*/

void Move::changeBoard(Board& other) const
{
  p = getPiece(other);
  c = getCell(other);
  b = &other;
}

const bool Move::doMove() const
{
  if(!b || !p)
    return false;
  
  bool ret;
  Cell * const orig = p->getCell();
  victim = c->getPiece();
  pFlag = p->hasFlag();
  vicFlag = victim? victim->hasFlag():-2;
  
  if(p == b->nextIn(p->getSide()))
  {
    b->playIn(p, c);
    ret = true;
  }
  else
    ret = b->move(p, c);
  
  c = orig;
  victim = ret? victim:NULL;
  //hadFlag = ret? hadFlag:-1;
  
  return ret;
}

const bool Move::undoMove() const
{
  if(!b || !p)
    return false;
  
  Cell * const orig = p->getCell();
  
  
  
  bool ret = b->unMove(p, c);
  if(victim != NULL)
  {
    ret = b->unMove(victim, orig);
    victim->hasFlag(vicFlag);
    b->setFlag(victim);
    p->hasFlag(pFlag);
    b->setFlag(p);
  }
  else
  {
    p->hasFlag(pFlag);
    b->setFlag(p);
  }
  c = orig;
  
  return ret;
}



BoardNode::BoardNode(const Board &bInit, AI * const ai): b(bInit), pov(ai), children(NULL)
{
  val = (pov != NULL? pov->evaluate(b):0.0);
}

const int BoardNode::generateChildren(const int depth)
{
  if(depth == 0 || b.won() != NULL)
  {
    children = NULL;
    return 0;
  }
  if(children != NULL)
    return generateChildren(depth - 1);
  
  children = new CHILD_LIST();
  
  vector<Piece> &pieces = b.getPieceList(b.whosTurn());
  for(auto i = pieces.begin(); i != pieces.end(); i++)
  {
    if(b.nextIn(b.whosTurn()))
    {
      if(b.nextIn(b.whosTurn()))
        continue;
    }
    auto validLocations = b.getValid(&*i);
    for(auto c = validLocations.begin(); c != validLocations.end(); c++)
    {
      ((*children)[Move(&*i, *c)] = new BoardNode(b, pov));
      auto temp = children->emplace(Move(&*i, *c, &b), new BoardNode(b, pov)).first;
      temp->first.changeBoard(temp->second->b);
      if(!temp->first.doMove())
	return -1;
    }
  }
  
  return 0;
}

const bool BoardNode::doMove(const Move& m)
{
  BoardNode * next;
  try
  {
    next = children->at(m);
  }
  catch(const out_of_range &oor)  // "sir, there's an exception"
  {
    return false;                 // "consider it handled"
  }
  
  b = next->getBoard();
  val = next->getValue();
  
  CHILD_LIST * temp = next->children;
  next->children = NULL;
  delete children;
  children = temp;
  
  return true;
}

BoardNode::~BoardNode()
{
  if(children)
  {
    for(auto i = children->begin(); i != children->end(); i++)
      delete i->second;
    delete children;
  }
}
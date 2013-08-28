#include "board.h"

const bool Board::move(Piece * const src, Cell * const dest)
{
  if(!src || !dest || !src->getCell())
    return false;
  
  if(canHandOff(src, dest))
  {
    dest->getPiece()->hasFlag(src->hasFlag());
    flags[src->hasFlag()] = dest->getPiece();
    src->hasFlag(-1);
    return true;
  }
  
  const vector<Cell *> validCells = getValid(src);
  if(none_of(validCells.begin(), //checks if the destination cell is valid
            validCells.end(), 
	    [dest](Cell *c) {return c == dest;}))
    return false;
  
  if(dest->getPiece())
  {
    if(dest->getPiece()->getSide() == src->getSide())
    {
      // attacking own piece
      // rules variables can change what happens here
      return false;
    }
    
    Piece * const dPiece = dest->getPiece();
    Piece * const sPiece = src;
    
    if(!canAttack(src, dest))
    {
      // attacking stronger piece
      // rules varibles can change what happens here
      return false;
    }
    else
    {
      ejectPiece(dPiece, sPiece->getStrength() - dPiece->getStrength());
    }
    
  }
  
  src->setCell(dest);
  
  
  if(dest->getType() == Cell::CAP_ZONE && dest->getSide() != src->getSide() &&
    flags[dest->getSide()] == NULL && src->hasFlag()<0)
  {	
    flags[dest->getSide()] = src;
    src->hasFlag(dest->getSide());
  }
  
  if(src->hasFlag() >= 0 && dest->getType() == Cell::CAP_ZONE &&
    src->getSide() == dest->getSide())
  {
    winner = src;
  }
  
  for(auto i = out.begin(); i != out.end(); i++)
  {
    if((*i)->getSide() == src->getSide())
      (*i)->turn();
  }
  
  return true;  
}

const bool Board::canAttack(Piece * const p, Cell * const c) const
{
  if(p == NULL || c == NULL)
    return false;
  
  if(c->getPiece() == NULL)
    return true;
  
  if(p->getSide() == c->getPiece()->getSide())
    return false;
  
  if(p->getStrength() < c->getPiece()->getStrength() ||
      (p->getStrength() == c->getPiece()->getStrength() &&
      c->getPiece()->getSide() == c->getSide()))
  {
    // attacking stronger piece
    // rules varibles can change what happens here
    return false;
  }
    
  return true;
}

const bool Board::canHandOff(Piece * const p, Cell * const c) const
{
  if(p == NULL || c == NULL)
    return false;
  
  if(c->getPiece() == NULL)
    return false;
  
  if(p == c->getPiece())
    return false;
  
  if(c->getPiece()->getSide() != p->getSide())
    return false;
    
  Mag_t dist = Vector(getPoint(p->getCell()) - getPoint(c)).mag();
  if(dist > 1.9 || dist < -1.9 || p->hasFlag() < 0 )
    return false;
  
  return true;
}

Piece * const Board::playIn(Piece * const p=NULL, Cell * const loc=NULL)
{
  if(p == NULL || loc == NULL)
    return NULL;
  
  if(loc->getType() != Cell::CAP_ZONE || loc->getSide() != p->getSide())
    return nextIn(p->getSide());
  
  if(loc->getPiece() || p->getCell())
    return nextIn(p->getSide());
  
  if(p->getOut())
    return nextIn(p->getSide());
  
  p->setCell(loc);
  
  out.erase(remove(out.begin(), out.end(), p), out.end());  
  
  return nextIn(p->getSide());
}

Piece * const Board::nextIn(const SIDE_t s) const
{
  auto rtrn = find_if(out.begin(), out.end(), [s](Piece *p) {
    return !p->getOut() && p->getSide()==s;
  });
  if(rtrn == out.end())
    return NULL;
  else
    return *rtrn;
}


const bool Board::handOff(Piece * const p1, Piece * const p2)
{
  Point rel = getPoint(p1->getCell()) - getPoint(p1->getCell());
  if(p1->hasFlag() < 0)
    return false;
  
  if(p2->hasFlag() >= 0)
    return false;
  
  if(p1->getSide() != p2->getSide())
    return false;
  
  if(!(rel.x() < 1.5 && rel.y() < 1.5 && rel.x() > -1.5 && rel.y() > -1.5))
    return false;
  
  p2->hasFlag(p1->hasFlag());
  p1->hasFlag(-1);
  
  flags[p2->hasFlag()] = p2;  
  
  return true;
}


void Board::ejectPiece(Piece * const p, const Uint8 o)
{
  p->setOut(o);
  out.push_back(p);
  p->getCell()->setPiece(NULL);
  p->setCell(NULL);
  if(p->hasFlag() >= 0)
  {
    flags[p->hasFlag()] = NULL;
    for(SIDE_t s = 0; s < sides; s++)
    {
      if(s == p->hasFlag())
	continue;
      for(vector<Piece>::iterator i = pieces[s].begin(); i != pieces[s].end(); i++)
      {
	if(i->getCell() == NULL)
	  continue;
        if(i->getCell()->getType() == Cell::CAP_ZONE &&
	  i->getCell()->getSide() == p->hasFlag())
        {
          i->hasFlag(p->hasFlag());
          flags[p->hasFlag()] = &*i;
	  break;
        }
      }
      if(flags[p->getSide()] != NULL)
	break;
    }
    p->hasFlag(-1);
  }
  
}

Cell * const Board::getCell(const Point &p) const
{
  if(p.x() < -0.5 || p.y() < -0.5 || p.x() > dimensions.x() - 0.5 || p.y() > dimensions.y() - 0.5)
    return NULL;
  
  return cells + ((int)((dimensions.x().val*p.y().val) + p.x().val));
}

Cell * const Board::getCell(Piece * const p) const
{
  return p->getCell();
}

const Point Board::getPoint(Cell * const c) const
{
  if(c - cells < 0 || c - cells > int(dimensions.x().val * dimensions.y().val))
    return Point(-1.0, -1.0);
  
  return Point((c - cells)%int(dimensions.x()), (c - cells) / int(dimensions.x()));
}

const vector<Cell *> Board::getValid(Cell * const c) const
{
  if(c->getPiece() == NULL)
    return vector<Cell *>(1, NULL);
  vector<Point> relative = c->getPiece()->getValid();
  vector<Cell *> rtrn;
  Cell *temp;
  Point pTemp;
  for(vector<Point>::iterator i=relative.begin(); i < relative.end(); i++)
  {
    if((pTemp = getPoint(c)).x() < -0.1)
      continue;
    if((temp = getCell(*i + pTemp)) == NULL)
      continue;
    if(temp->getPiece() != NULL)
    {
      if(!canAttack(c->getPiece(), temp) && !canHandOff(c->getPiece(), temp))
        continue;
    }
    rtrn.push_back(temp);
  }
  
  return rtrn;
}

const vector<Cell *> Board::getValid(Piece * const p) const
{
  return getValid(p->getCell());
}

Board::Board(const Dimensions d, const SIDE_t nSides, 
	     const Uint8 nP, Cell * const c, vector<Piece> * const p):
	     dimensions(d), sides(nSides), nPieces(nP), cells(c), pieces(p), winner(NULL)
{
  if(cells == NULL && nSides == 2)
    cells = Cell::generateDefaultCells(dimensions);
  
  if(cells == NULL)
    return;
  
  if(pieces == NULL)
  {
    pieces = new vector<Piece> [sides];
    if(pieces == NULL)
      return;
    for(SIDE_t s = 0; s < sides; s++)
    {
      pieces[s] = Piece::generatePieces(s, nPieces);
    }
  }
  
  flags = new Piece * [sides];
  for(Uint8 i = 0; i < sides; i++)
    flags[i] = NULL;
  
  for(Uint8 i = 0; i < sides; i++)
  {
    for(auto j = pieces[i].begin(); j != pieces[i].end(); j++)
    {
      out.push_back(&*j);
    }
  }
}

Board::~Board()
{
  delete [] cells;
  delete [] pieces;
  delete [] flags;
}

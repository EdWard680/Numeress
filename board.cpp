#include "board.h"

const char * const Board::toString() const
{
  if(cells == NULL || pieces == NULL || flags == NULL)
    return NULL;
#ifdef DEBUG
  for(Uint8 y = 0; y < 10; y++)
  {
    for(Uint8 x = 0; x < 10; x++)
    {
      boardString[y][x*2] = (cells[y*10 + x].getPiece() == NULL? '-':('0' + cells[y*10 + x].getPiece()->getStrength()));
      boardString[y][x*2 + 1] = ' ';
    }
    boardString[y][19] = NULL;
  }
  return &boardString[0][0];
#endif
}


const bool Board::move(Piece * const src, Cell * const dest)
{
  if(!src || !dest || !src->getCell() || !cellHere(dest) || !cellHere(src==NULL? NULL:src->getCell()))
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
  
  turn++;
  moved(src, dest);
  return true;
}

const bool Board::unMove(Piece * const src, Cell * const dest)
{
  winner = NULL;
  if(dest == NULL)
  {
    for(auto o = out.begin(); o != out.end(); o++)
    {
      (*o)->setOut((*o)->getOut() + 1);
    }
    ejectPiece(src, 0);
  }
  else if(src->getCell() == NULL)
  {
    src->setCell(dest);
    src->setOut(0);
    out.erase(remove(out.begin(), out.end(), src), out.end());
  }
  else
  {
    for(auto o = out.begin(); o != out.end(); o++)
    {
      (*o)->setOut((*o)->getOut() + 1);
    }
    src->setCell(dest);
    if(src->hasFlag() >= 0)
    {
      flags[src->hasFlag()] = NULL;
      src->hasFlag(-1);
    }
    turn-=1;
  }
  
  return true;
}

const bool Board::canAttack(Piece * const p, Cell * const c) const
{
  if(p == NULL || c == NULL || !cellHere(c) || !cellHere(p==NULL? NULL:p->getCell()))
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
  if(p == NULL || c == NULL || !cellHere(c) || !cellHere(p==NULL? NULL:p->getCell()))
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
  if(p == NULL || loc == NULL || !cellHere(loc))
    return NULL;
  
  if(loc->getType() != Cell::CAP_ZONE || loc->getSide() != p->getSide())
    return nextIn(p->getSide());
  
  if(loc->getPiece() || p->getCell())
    return nextIn(p->getSide());
  
  if(p->getOut())
    return nextIn(p->getSide());
  
  p->setCell(loc);
  
  out.erase(remove(out.begin(), out.end(), p), out.end());  
  
  playedIn(p, loc);
  
  return nextIn(p->getSide());
}

Piece * const Board::nextIn(const SIDE_t s) const
{
  if(cells == NULL)
    return NULL;
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
  if(cells == NULL || p1 == NULL || p2 == NULL)
    return false;
  if(!cellHere(p1->getCell()))
    return false;
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
  
  handedOff(p1, p2);
  
  return true;
}


int Board::ejectPiece(Piece * const p, const Uint8 o)
{
  if(p == NULL || cells == NULL)
    return -1;
  p->setOut(o);
  out.push_back(p);
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
          flags[p->hasFlag()] = &(*i);
	  break;
        }
      }
      if(flags[p->hasFlag()] != NULL)
	break;
    }
    p->hasFlag(-1);
  }
  
  ejectedPiece(p, o);
  
  return 0;
}

Cell * const Board::getCell(const Point &p) const
{
  if(cells == NULL || p.x() < -0.5 || p.y() < -0.5 || p.x() > dimensions.x() - 0.5 || p.y() > dimensions.y() - 0.5)
    return NULL;
  
  return cells + ((int)((dimensions.x().val*p.y().val) + p.x().val));
}

Cell * const Board::getCell(Piece * const p) const
{
  return p->getCell();
}

const Point Board::getPoint(Cell * const c) const
{
  if(!cellHere(c) || c - cells < 0 || c - cells > int(dimensions.x().val * dimensions.y().val))
    return Point(-1.0, -1.0);
  
  return Point((c - cells)%int(dimensions.x()), (c - cells) / int(dimensions.x()));
}

const vector<Cell *> Board::getValid(Piece * const p) const
{
  if(p == NULL || cells == NULL)
    return vector<Cell *>(1, NULL);
  else if(p->getOut() == 0 && p->getCell() == NULL)
  {
    vector<Cell *> rtrn;
    for(Uint8 i = 0; i < dimensions.x().val * dimensions.y().val; i++)
    {
      if(cells[i].getType() == Cell::CAP_ZONE && cells[i].getSide() == p->getSide() && cells[i].getPiece() == NULL)
	rtrn.push_back(cells + i);
    }
    
    return rtrn;
  }
  vector<Point> relative = p->getValid();
  vector<Cell *> rtrn;
  Cell *temp;
  Point pTemp;
  for(vector<Point>::iterator i=relative.begin(); i != relative.end(); i++)
  {
    if((pTemp = getPoint(p->getCell())).x() < -0.1)
      continue;
    if((temp = getCell(*i + pTemp)) == NULL)
      continue;
    if(temp->getPiece() != NULL)
    {
      if(!canAttack(p, temp) && !canHandOff(p, temp))
        continue;
    }
    rtrn.push_back(temp);
  }
  
  return rtrn;
}

const vector<Cell *> Board::getValid(Cell * const c) const
{
  if(c == NULL || !cellHere(c))
    return vector<Cell *>(1, NULL);
  return getValid(c->getPiece());
}

const bool Board::setFlag(Piece * const p)
{
  if(p == NULL)
    return false;
  
  if(!cellHere(p->getCell()))
    return false;
  
  if(p->hasFlag() >= 0 && p->hasFlag() < sides)
  {
    flags[p->hasFlag()] = p;
    return true;
  }
  else if(p->hasFlag() == -1)
  {
    for(Uint8 i = 0; i < sides; i++)
    {
      if(flags[i] == p)
	flags[i] = NULL;
    }
    
    return true;
  }
  
  return false;
}



Board::Board(const Board& other): 
dimensions(other.dimensions), sides(other.sides), nPieces(other.nPieces), turn(other.turn), winner(other.winner)
{
  cells = new Cell [int(dimensions.x().val * dimensions.y().val)];
  memcpy(cells, other.cells, sizeof(Cell) * int(dimensions.x().val*dimensions.y().val));
  for(Uint8 c = 0; c < dimensions.x().val * dimensions.y().val; c++)
    cells[c].setPiece(NULL);
  pieces = new vector<Piece> [sides];
  flags = new Piece* [sides];
  for(SIDE_t s = 0; s < sides; s++)
  {
    pieces[s].reserve(other.pieces[s].size());
    pieces[s] = other.pieces[s];
    for(Uint8 p = 0; p < pieces[s].size(); p++)
    {
      if(other.pieces[s][p].getCell() != NULL)
	pieces[s][p].setCell(cells + (other.pieces[s][p].getCell() - other.cells));
      
      if(pieces[s][p].getCell() == NULL)
	out.push_back(&pieces[s][p]);
    }
  }
  
  for(Uint8 s = 0; s < sides; s++)
  {
    if(other.flags[s] != NULL)
      flags[s] = getPiece(other.flags[s]->getValue(), other.flags[s]->getSide());
    else
      flags[s] = NULL;
  }
  
  if(other.winner != NULL)
    winner = getPiece(other.winner->getValue(), other.winner->getSide());
  
  toString();
}

Board &Board::operator= (Board other)
{
  swap(this->winner, other.winner);
  swap(this->cells, other.cells);
  swap(this->pieces, other.pieces);
  swap(this->flags, other.flags);
  swap(this->out, other.out);
  swap(this->turn, other.turn);
  toString();
  return *this;
}

Board::Board(const Dimensions d, const SIDE_t nSides, 
	     const Uint8 nP, Cell * const c, vector<Piece> * const p, const bool first):
	     dimensions(d), sides(nSides), nPieces(nP), cells(c), pieces(p), winner(NULL), turn(first? 1:0)
{
  if(cells == NULL && nSides == 2)
  {
    cells = Cell::generateDefaultCells(dimensions);
  }
  
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
    
    for(Uint8 i = 0; i < sides; i++)
    {
      for(auto j = pieces[i].begin(); j != pieces[i].end(); j++)
      {
        out.push_back(&*j);
      }
    }
  
  }
  
  flags = new Piece * [sides];
  for(Uint8 i = 0; i < sides; i++)
    flags[i] = NULL;
  
  toString();
}

Board::~Board()
{
  if(cells)
    delete [] cells;
  if(pieces)
    delete [] pieces;
  if(flags)
    delete [] flags;
}

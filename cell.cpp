#include "cell.h"
#include "piece.h"

int Cell::prev(0);

Cell::Cell(const Uint8 t, const int s): type(s==-1? prev:(s<<1 | t)), piece(NULL)
{
  prev = type;
}

Piece * const Cell::setPiece(Piece * const p)
{
  return piece = p;
}
/*
Cell& Cell::operator= (const Cell& c)
{
  Cell copy(c);
  swap(this->type, copy.type);
  swap(this->piece, copy.piece);
  //new (this) Cell(c.getType(), c.getSide());
  //Apparently this is not "exception safe". Lol whatever that means
  return *this;
}
*/
Cell * const Cell::generateDefaultCells(Dimensions d)
{
  Cell * const rtrn = new Cell [(int)d.x().val * (int)d.y().val];
  const Cell c1(CAP_ZONE, 0), c2(NEUTRAL, 0), c3(NEUTRAL, 1), c4(CAP_ZONE, 1);
  Uint8 i, j;
  for(i = 0; i < 2 * d.x(); i++)
  {
    rtrn[i] = c1;
  }
  
  for(j = i; j < i + 3 * d.x(); j++)
  {
    rtrn[j] = c2;
  }
  
  for(i = j; i < j + 3 * d.x(); i++)
  {
    rtrn[i] = c3;
  }
  
  for(j = i; j < i + 2 * d.x(); j++)
  {
    rtrn[j] = c4;
  }
  
  return rtrn;
}
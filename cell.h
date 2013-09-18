#ifndef _CELL_H_
#define _CELL_H_

#include "header.h"

class Cell
{
public:
  enum {NEUTRAL=0x0, CAP_ZONE=0x1};
  static Cell * const generateDefaultCells(const Dimensions d);
  
private: 
  static int prev;
  
private:
  int type;
  Piece *piece;  
  
public:
  const Uint8 getType() const {return type & CAP_ZONE;};
  const SIDE_t getSide() const {return type >> 1;};
  Piece * const setPiece(Piece * const p); //{return piece = p;};
  Piece * const getPiece() const {return piece;};
  //const operator CELL_t () const {return type;};
  
public:
  Cell(const Uint8 t=false, const int s=-1);
  Cell(const Cell &cp): type(cp.type), piece(NULL) {};
  //Cell& operator= (const Cell& c);
};



#endif
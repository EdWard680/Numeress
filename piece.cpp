#include "piece.h"
#include "cell.h"

Cell * const Piece::setCell(Cell * const c)
{
  location != NULL? (location->setPiece(NULL)):(NULL);
  c != NULL? (c->setPiece(this)):(NULL);
  return location = c;
}

char * const Piece::getFileName() const 
{
  char *rtrn = new char [sizeof("moves/1_1.txt")];
  sprintf(rtrn, "moves/%i_%i.txt", side, strength);  
  return rtrn;
}

const vector<Point> Piece::generateValid()
{
  char *fileName = getFileName();
  FILE *fp = fopen(fileName, "r");
  if(fp == NULL)
    return valid;
  
  bool cont = true;
  int xTemp, yTemp;
  while(fscanf(fp, "(%i,%i)\n", &xTemp, &yTemp) != EOF)
    valid.push_back(Point(xTemp, yTemp));
  
  delete [] fileName;
  
  fclose(fp);
  return valid;
}

const Uint8 Piece::getStrength() const
{
  return flag>=0? 0:strength;
}


Piece::Piece(const SIDE_t sd, const Uint8 sgt, Cell * const initLoc): 
strength(sgt), 
side(sd),
location(initLoc),
flag(-1),
out(0)
{
  generateValid();
}

Piece& Piece::operator= (const Piece& p)
{
  new (this) Piece(p.getStrength(), getSide(), NULL);
  return *this;
}

vector<Piece> Piece::generatePieces(const SIDE_t side, const Uint8 pieces=5)
{
  vector<Piece> rtrn;
  for(Uint8 i = 0; i < pieces; i++)
  {
    rtrn.push_back(Piece(side, i+1, NULL));
  }
  
  return rtrn;
}
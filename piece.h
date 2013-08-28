#ifndef _PIECE_H_
#define _PIECE_H_

#include "header.h"

class Piece
{
public:
  static vector<Piece> generatePieces(const SIDE_t s, const Uint8 pieces);
  
private:
  const Uint8 strength;  // Piece value.
  const SIDE_t side;     // Side piece belongs.
  int flag;   // Whether it has the flag or not.
  Uint8 out;  // Number of turns it must remains out of play.
  Cell *location;  // Pointer to cell it rests on.
  vector<Point> valid;  // List of possible moves the piece can make.
  char * const getFileName() const;  // Returns file name used to make list of possible moves
  const vector<Point> generateValid();  // Returns pointer to generated list of valid moves.
  
public:
  const Uint8 getStrength() const;
  const Uint8 getValue() const {return strength;};
  const SIDE_t getSide() const {return side;};
  const int hasFlag() const {return flag;};
  const int hasFlag(const int f) {return flag = f;};
  Cell * const getCell() const {return location;};
  Cell * const setCell(Cell * const c);// {return location = c;};
  const Uint8 getOut() const {return out;};
  const Uint8 setOut(const Uint8 o) {return out=o;};
  const Uint8 turn() {return out>0? out--:out;};
  vector<Point> const getValid() const {return valid;};
  
public:
  Piece(const Uint8 sgt, const SIDE_t sd, Cell * const initLoc);
  Piece& operator=(const Piece& p);
};


#endif
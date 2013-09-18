#include "board.h"

void display(const Board &b)
{
  Piece *temp;
  const char * const merp = b.toString();
  cout<<endl<<endl<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < b.getDim().y(); y++)
  {
    cout<<char('A'+y)<<' '<<(merp + y*20)<<endl;
  }
  //cout<<"Value for current game: "<<myAI.evaluate(b)<<endl;
  //cout<<"Expected Value for future of game: "<<myAI.getBestMove().prev<<endl;
}

void permute(Board b, Uint8 depth)
{
  display(b);
  Board next;
  if(depth == 0)
    return;
  vector<Piece> &pieces = b.getPieceList(b.whosTurn());
  for(auto i = pieces.begin(); i != pieces.end(); i++)
  {
    Piece *temp;
    if(temp = b.nextIn(b.whosTurn()))
    {
      if(temp != &*i)
	continue;
    }
    auto validLocations = b.getValid(&*i);
    for(auto c = validLocations.begin(); c != validLocations.end(); c++)
    {
      next = b;
      if(temp)
      {
	if(!next.playIn(next.getPiece(i->getValue(), i->getSide()), next.getCell(b.getPoint(*c))))
	{
	  continue;
	}
      }
      else
      {
	if(!next.move(next.getPiece(i->getValue(), i->getSide()), next.getCell(b.getPoint(*c))))
	{
	  cout<<"Valid Move Failed"<<endl;
	  cout<<"Piece "<<int(i->getValue())<<" of side "<<int(i->getSide())<<endl;
	  cout<<"Moving from "<<char('A' + int(b.getPoint(i->getCell()).y()))<<int(b.getPoint(i->getCell()).x());
	  cout<<" to "<<char('A' + int(b.getPoint(*c).y()))<<int(b.getPoint(*c).x())<<endl;
	  cin.clear();
	  cin.ignore(9001, '\n');
	  continue;
	}
      }
      
      permute(next, depth - 1);
      
    }
  }
}

int main(void)
{
  Board b(Dimensions(10.0, 10.0), 2, 5);
  b.playIn(b.getPiece(1, 0), b.getCell(Point(4, 0)));
  b.playIn(b.getPiece(1, 1), b.getCell(Point(5, 9)));
  b.playIn(b.getPiece(2, 0), b.getCell(Point(8, 0)));
  b.playIn(b.getPiece(2, 1), b.getCell(Point(1, 9)));
  b.playIn(b.getPiece(3, 0), b.getCell(Point(2, 0)));
  b.playIn(b.getPiece(3, 1), b.getCell(Point(7, 9)));
  b.playIn(b.getPiece(4, 0), b.getCell(Point(0, 0)));
  b.playIn(b.getPiece(4, 1), b.getCell(Point(9, 9)));
  b.playIn(b.getPiece(5, 0), b.getCell(Point(6, 0)));
  b.playIn(b.getPiece(5, 1), b.getCell(Point(3, 9)));
  
  permute(b, 5);
  
}
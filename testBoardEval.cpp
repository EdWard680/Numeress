#include "AI.h"

void display(const Board &b, AI &myAI)
{
#ifdef DEBUG
  Piece *temp;
  const char * const merp = b.toString();
  cout<<endl<<endl<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < b.getDim().y(); y++)
  {
    cout<<char('A'+y)<<' '<<(merp + y*20)<<endl;
  }
  cout<<"Value for current game: "<<myAI.evaluate(b)<<endl;
#else
  Piece *temp;
  cout<<endl<<endl<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < b.getDim().y(); y++)
  {
    cout<<char(y+'A')<<' ';
    for(Uint8 x = 0; x < b.getDim().x(); x++)
    {
      if((temp = b.getCell(Point(x, y))->getPiece()) == NULL)
	cout<<"- ";
      else
	cout<<int(temp->getStrength())<<' ';
    }
    cout<<endl;
  }
  cout<<"Value for current game: "<<myAI.evaluate(b)<<endl;
#endif
  //cout<<"Expected Value for future of game: "<<myAI.getBestMove().prev<<endl;
}

const Point retrievePoint(const Dimensions &d)
{
  int xTemp;
  char yTemp;
  cin.clear();
  cin.ignore(9001, '\n');
  cout<<endl;
  do
  {
    cout<<"Coordinates: ";
    scanf("%c%d", &yTemp, &xTemp);
    yTemp -= 'A';
  } while(xTemp < 0 || xTemp > d.x()-0.5 || yTemp < 0 || yTemp > d.y()-0.5);
  
  return Point(xTemp, yTemp);
}

const Uint8 retrievePiece(const Uint8 nPieces)
{
  int rtrn;
  cin.clear();
  cin.ignore(9001, '\n');
  do
  {
    cout<<"Piece Value: ";
    cin>>rtrn;
  } while(rtrn < 0 || rtrn > nPieces);
  return rtrn;
}

void playIns(Board &b, const SIDE_t s, AI &myAI)
{
  Piece * next;
  Move n;
  while(next = b.nextIn(s))
  {
    display(b, myAI);
    cout<<endl<<"Player "<<int(s)<<": Play in piece "<<int(next->getStrength())<<endl;
    if(s == myAI.getSide())
    {
      if(next == (n = myAI.getBestMove(5)).getPiece())
      {
	b.playIn(next, n.getCell());
      }
    }
    else
    {
      b.playIn(next, b.getCell(retrievePoint(b.getDim())));
    }
  }
}

int main(void)
{
  Piece * winner;
  vector<Piece> * startPieces = new vector<Piece> [2];
  Cell * startCells = Cell::generateDefaultCells(Dimensions(10.0, 10.0));
  startPieces[0] = Piece::generatePieces(0);
  startPieces[1] = Piece::generatePieces(1);
  
  startPieces[1][0].setCell(startCells + 55);
  startPieces[1][1].setCell(startCells + 86);
  startPieces[1][2].setCell(startCells + 29);
  startPieces[1][3].setCell(startCells + 88);
  startPieces[1][4].setCell(startCells + 92);
  startPieces[0][0].setCell(startCells + 15);
  startPieces[0][1].setCell(startCells + 10);
  startPieces[0][2].setCell(startCells + 56);
  startPieces[0][3].setCell(startCells + 0);
  startPieces[0][4].setCell(startCells + 28);
  
  Board b(Dimensions(10.0, 10.0), 2, 5, startCells, startPieces, 1);
  startPieces[1][2].hasFlag(0);
  b.setFlag(&startPieces[1][2]);
  
  AI myAI(&b, 0), myAI2(&b, 1);
//   for(SIDE_t s = 0; s < b.getSides(); s++)
//     playIns(b, s);  //lol
  
  /*
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
  */
  
  while(!(winner = b.won()))
  {
    cout<<"Turn "<<int(b.getTurn())<<endl;
    cout<<"Player "<<int(b.whosTurn())<<endl;
    display(b, myAI);
    playIns(b, b.whosTurn(), b.whosTurn()? myAI2:myAI);
    display(b, myAI);
    Piece *p;
    Cell *c;
    do
    {
      cout<<"Move Player "<<int(b.whosTurn())<<endl;
      if(b.whosTurn() == myAI.getSide())
      {
	Move n(myAI.getBestMove(5));
	p = n.getPiece();
	c = n.getCell();
      }
      else
      {
	/*
	p = b.getPiece(retrievePiece(5), 1);
	c = b.getCell(retrievePoint(b.getDim()));
	*/
	Move n(myAI2.getBestMove(5));
	p = n.getPiece();
	c = n.getCell();
	
      }
      
      
    } while(!b.move(p, c));
  }
  
  display(b, myAI);
  cout<<endl<<"Player "<<int(b.won()->getSide())<<" capped"<<endl;
  return 0;
}